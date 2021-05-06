#include "pagecontroller.h"
#include "classicimagerunnable.h"
#include "simpleimagerunnable.h"
#include "utils/imageproc.h"
#include "parsers/parser.h"
#define PRIORITY_MAX 0
#define PRIORITY_REQ 1

PageController::PageController(const QUrl& book_filename, bool toram, int imgprld, QObject *parent)
    : QObject(parent),
      book(book_filename, toram),
      imagePreload(imgprld < 0 ? book.size() : imgprld)
{}

PageController::~PageController() {
    pool.clear();
    pool.waitForDone();
    qWarning("controller deleted");
}

void PageController::getAsyncPage(PageRequest req, PageAnswer* ans) {
    int index = req.index();
    int book_size = book.size();
    if (index<0 || index >= book_size) {
        ans->answer(QImage());;
        return;
    }
    if (pages.value(req).matchStatus(RequestStatus::Recieved)) {
        qWarning("Controller: already reviced %i", req.index());
        ans->answer(pages.value(req).img);
    } else if (pages.value(req).matchStatus(RequestStatus::Requested)) {
        qWarning("Controller: already requested, add to pending %i", req.index());
        pendingReqs.insert(req, ans);
    } else {
        qWarning("Controller: requesting, add to pending %i", req.index());
        pendingReqs.insert(req, ans);
        //pool.clear();
        runPage(req, PRIORITY_MAX);
        //runLocalPage(req);
    }
    preloadPages(req);
    clearPages(req);
}

void PageController::preloadPages(PageRequest req) {
    int index = req.index();
    int book_size = book.size();
    for (int i=1; i<imagePreload; i++) {
        PageRequest preq = req.addIndex(i);
        PageRequest mreq = req.addIndex(-i);
        if (index+i<book_size && pages.value(preq).matchStatus(RequestStatus::Undefined)) {
            qWarning("%i", preq.index());
            runPage(preq, PRIORITY_REQ);
            //runLocalPage(preq);
        }
        if (index-i>=0 && pages.value(mreq).matchStatus(RequestStatus::Undefined)) {
            qWarning("%i", mreq.index());
            runPage(mreq, PRIORITY_REQ);
            //runLocalPage(mreq);
        }
    }
}

void PageController::clearPages(PageRequest req) {
    QList<PageRequest> k = pages.keys();
    for (QList<PageRequest>::iterator it=k.begin(); it !=k.end(); ++it) {
        if(not it->isLike(req) || not it->isInRange(req, imagePreload)) {
            pages.remove(*it);
        }
    }
}

void PageController::runPage(PageRequest req, int priority) {
    pages.insert(req, Pair{RequestStatus::Requested, QImage()});
    if (req.runnableType()=="classic") {
        ClassicImageRunnable *runnable = new ClassicImageRunnable(book, req);
        connect(runnable, &ClassicImageRunnable::done, this, &PageController::handleImage);
        pool.start(runnable, priority);
    } else if (req.runnableType()=="simple") {
        SimpleImageRunnable *runnable = new SimpleImageRunnable(book, req);
        connect(runnable, &SimpleImageRunnable::done, this, &PageController::handleImage);
        pool.start(runnable, priority);
    }
}

void PageController::runLocalPage(PageRequest req) {
    pages.insert(req, Pair{RequestStatus::Requested, QImage()});
    ClassicImageRunnable *runnable = new ClassicImageRunnable(book, req);
    connect(runnable, &ClassicImageRunnable::done, this, &PageController::handleImage);
    runnable->run();
    runnable->deleteLater();
}

QUrl PageController::getBookFilename() {
    return book.filename();
}

void PageController::handleImage(PageRequest req, QImage img) {
    //pendingReqs.isEmpty() ? qWarning("Controller: recieved!!! %i %i %i pending: None", req.index(), req.width(), req.height()): qWarning("Controller: recieved!!! %i %i %i pending: %i %i %i", req.index(), req.width(), req.height(), pendingReqs.head().index(), pendingReqs.head().width(), pendingReqs.head().height());
    if (pages.value(req).matchStatus(RequestStatus::Requested)) {
        pages.insert(req, Pair{RequestStatus::Recieved, img});
    }
    if (pendingReqs.contains(req)) {
        qWarning("Controller: answaring: %i, (%i, %i)", req.index(), req.width(), req.height());
        pendingReqs.value(req)->answer(img);
        pendingReqs.remove(req);
    }
}
