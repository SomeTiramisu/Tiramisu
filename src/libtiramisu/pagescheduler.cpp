#include "pagescheduler.h"
#include "runnables/cropscalerunnable.h"
#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler(PagePreloader* preloader, QObject *parent)
    : QObject(parent),
      m_preloader(preloader),
      m_imagePreload(20)
{}

PageScheduler::~PageScheduler() {
    m_pool.clear();
    m_pool.waitForDone();
    qWarning("scheduler deleted");
}

QImage PageScheduler::getAsyncPage(PageRequest req) {
    int index = req.index();
    int book_size = m_preloader->size();
    if (index<0 || index >= book_size) {
        return QImage();
    }
    if (m_pages.value(req).matchStatus(RequestStatus::Recieved)) {
        qWarning("Controller: already reviced %i", req.index());
    } else if (m_pages.value(req).matchStatus(RequestStatus::Requested)) {
        qWarning("Controller: already requested, rerun local %i", req.index());
        m_pendingReqs.remove(req);
        m_pages.insert(req, PagePair{RequestStatus::Recieved, runLocalPage(req)});
    } else {
        qWarning("Controller: running locally %i", req.index());
        m_pages.insert(req, PagePair{RequestStatus::Recieved, runLocalPage(req)});
    }
    preloadPages(req);
    clearPages(req);
    return m_pages.value(req).img;;
}

void PageScheduler::preloadPages(PageRequest req) {
    int index = req.index();
    int book_size = m_preloader->size();
    for (int i=1; i<m_imagePreload; i++) {
        PageRequest preq = req.addIndex(i);
        PageRequest mreq = req.addIndex(-i);
        if (index+i<book_size && m_pages.value(preq).matchStatus(RequestStatus::Undefined)) {
            //qWarning("%i", preq.index());
            runPage(preq, RequetPriority::Req);
            //runLocalPage(preq);
        }
        if (index-i>=0 && m_pages.value(mreq).matchStatus(RequestStatus::Undefined)) {
            //qWarning("%i", mreq.index());
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
    m_pages.insert(req, PagePair{RequestStatus::Requested, QImage()});
    CropScaleRunnable *runnable = new CropScaleRunnable(m_preloader, req);
    connect(runnable, &CropScaleRunnable::imageReady, this, &PageScheduler::handleImage);
    m_pool.start(runnable, priority);
}

QImage PageScheduler::runLocalPage(PageRequest req) {
    CropScaleRunnable runnable = CropScaleRunnable(m_preloader, req);
    return runnable.runLocal();
}

void PageScheduler::handleImage(PageRequest req, QImage img) {
    //pendingReqs.isEmpty() ? qWarning("Controller: recieved!!! %i %i %i pending: None", req.index(), req.width(), req.height()): qWarning("Controller: recieved!!! %i %i %i pending: %i %i %i", req.index(), req.width(), req.height(), pendingReqs.head().index(), pendingReqs.head().width(), pendingReqs.head().height());
    qWarning("Handled Sheduler");
    if (m_pages.value(req).matchStatus(RequestStatus::Requested)) {
        m_pages.insert(req, PagePair{RequestStatus::Recieved, img});
    }
    if (m_pendingReqs.contains(req)) {
        qWarning("Controller: answaring: %i, (%i, %i)", req.index(), req.width(), req.height());
        emit imageReady(req, img);
        m_pendingReqs.remove(req);
    }
}
