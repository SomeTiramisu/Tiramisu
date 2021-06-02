#include "pagescheduler.h"
#include "classicimagerunnable.h"
#include "simpleimagerunnable.h"
#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler(const QUrl& filename, bool toRam, int imgPrld, QObject *parent)
    : QObject(parent),
      m_parser(filename, toRam),
      m_imagePreload(imgPrld < 0 ? m_parser.size() : imgPrld)
{}

PageScheduler::~PageScheduler() {
    m_pool.clear();
    m_pool.waitForDone();
    qWarning("controller deleted");
}

void PageScheduler::getAsyncPage(PageRequest req, PageAnswer* ans) {
    int index = req.index();
    int book_size = m_parser.size();
    if (index<0 || index >= book_size) {
        ans->answer(QImage());;
        return;
    }
    if (m_pages.value(req).matchStatus(RequestStatus::Recieved)) {
        qWarning("Controller: already reviced %i", req.index());
        ans->answer(m_pages.value(req).img);
    } else if (m_pages.value(req).matchStatus(RequestStatus::Requested)) {
        qWarning("Controller: already requested, add to pending %i", req.index());
        m_pendingReqs.insert(req, ans);
    } else {
        qWarning("Controller: requesting, add to pending %i", req.index());
        m_pendingReqs.insert(req, ans);
        //pool.clear();
        runPage(req, RequetPriority::Max);
        //runLocalPage(req);
    }
    preloadPages(req);
    clearPages(req);
}

void PageScheduler::preloadPages(PageRequest req) {
    int index = req.index();
    int book_size = m_parser.size();
    for (int i=1; i<m_imagePreload; i++) {
        PageRequest preq = req.addIndex(i);
        PageRequest mreq = req.addIndex(-i);
        if (index+i<book_size && m_pages.value(preq).matchStatus(RequestStatus::Undefined)) {
            qWarning("%i", preq.index());
            runPage(preq, RequetPriority::Req);
            //runLocalPage(preq);
        }
        if (index-i>=0 && m_pages.value(mreq).matchStatus(RequestStatus::Undefined)) {
            qWarning("%i", mreq.index());
            runPage(mreq, RequetPriority::Req);
            //runLocalPage(mreq);
        }
    }
}

void PageScheduler::clearPages(PageRequest req) {
    QList<PageRequest> k = m_pages.keys();
    for (QList<PageRequest>::iterator it=k.begin(); it !=k.end(); ++it) {
        if(not it->isLike(req) || not it->isInRange(req, m_imagePreload)) {
            m_pages.remove(*it);
        }
    }
}

void PageScheduler::runPage(PageRequest req, RequetPriority priority) {
    m_pages.insert(req, Pair{RequestStatus::Requested, QImage()});
    if (req.runnableType()=="classic") {
        ClassicImageRunnable *runnable = new ClassicImageRunnable(m_parser, req);
        connect(runnable, &ClassicImageRunnable::done, this, &PageScheduler::handleImage);
        m_pool.start(runnable, priority);
    } else if (req.runnableType()=="simple") {
        SimpleImageRunnable *runnable = new SimpleImageRunnable(m_parser, req);
        connect(runnable, &SimpleImageRunnable::done, this, &PageScheduler::handleImage);
        m_pool.start(runnable, priority);
    }
}

void PageScheduler::runLocalPage(PageRequest req) {
    m_pages.insert(req, Pair{RequestStatus::Requested, QImage()});
    ClassicImageRunnable *runnable = new ClassicImageRunnable(m_parser, req);
    connect(runnable, &ClassicImageRunnable::done, this, &PageScheduler::handleImage);
    runnable->run();
    runnable->deleteLater();
}

QUrl PageScheduler::getBookFilename() {
    return m_parser.filename();
}

void PageScheduler::handleImage(PageRequest req, QImage img) {
    //pendingReqs.isEmpty() ? qWarning("Controller: recieved!!! %i %i %i pending: None", req.index(), req.width(), req.height()): qWarning("Controller: recieved!!! %i %i %i pending: %i %i %i", req.index(), req.width(), req.height(), pendingReqs.head().index(), pendingReqs.head().width(), pendingReqs.head().height());
    if (m_pages.value(req).matchStatus(RequestStatus::Requested)) {
        m_pages.insert(req, Pair{RequestStatus::Recieved, img});
    }
    if (m_pendingReqs.contains(req)) {
        qWarning("Controller: answaring: %i, (%i, %i)", req.index(), req.width(), req.height());
        m_pendingReqs.value(req)->answer(img);
        m_pendingReqs.remove(req);
    }
}
