#include "pagecontroller.h"
#include "imagerunnable.h"
#include "utils/imageproc.h"
#include "parsers/parser.h"
#define IMAGE_PRELOAD 10
#define PRIORITY_MAX 0
#define PRIORITY_REQ 1

PageController::PageController(QUrl book_filename, QObject *parent)
    : QObject(parent),
      book(book_filename, true)
{}

PageController::~PageController() {
    pool.clear();
    pool.waitForDone();
    qWarning("controller deleted");
}

void PageController::getAsyncPage(PageRequest req) {
    int index = req.index;
    int book_size = book.getSize();
    pendingReq = PageRequest();
    if (index<0 || index >= book_size) {
        emit pageReady(QImage());;
        return;
    }
    if (pages.value(req).matchStatus(RequestStatus::Recieved)) {
        emit pageReady(pages.value(req).img);
    } else if (pages.value(req).matchStatus(RequestStatus::Requested)) {
        pendingReq = req;
    } else {
        pendingReq = req;
        pool.clear();
        runPage(req, PRIORITY_MAX);
    }
    preloadPages(req);
}

void PageController::preloadPages(PageRequest req) {
    int index = req.index;
    int book_size = book.getSize();
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        PageRequest new_req(req);
        PageRequest preq = new_req.addIndex(i);
        PageRequest mreq = new_req.addIndex(-i);
        if (index+i<book_size && pages.value(preq).matchStatus(RequestStatus::Undefined)) {
            runPage(preq, PRIORITY_REQ);
        }
        if (index-i>=0 && pages.value(mreq).matchStatus(RequestStatus::Undefined)) {
            runPage(mreq, PRIORITY_REQ);
        }
    }

    QList<PageRequest> k = pages.keys();
    for (QList<PageRequest>::iterator it=k.begin(); it !=k.end(); ++it) {
        if(not it->isLike(req) || not it->isInRange(req, IMAGE_PRELOAD)) {
            pages.remove(*it);
        }
    }
}

void PageController::runPage(PageRequest req, int priority) {
    pages.insert(req, Pair{RequestStatus::Requested, QImage()});
    ImageRunnable *runnable = new ImageRunnable(book, req);
    connect(runnable, &ImageRunnable::done, this, &PageController::handleImage);
    pool.start(runnable, priority);
}

void PageController::runLocalPage(PageRequest req) {
    ImageRunnable *runnable = new ImageRunnable(book, req);
    connect(runnable, &ImageRunnable::done, this, &PageController::handleImage);
    runnable->run();
    runnable->deleteLater();
}

QUrl PageController::getBookFilename() {
    return book.getFilename();
}

void PageController::handleImage(PageRequest req, QImage img) {
    qWarning("Controller: recieved!!! %i %i %i pending: %i %i %i", req.index, req.width, req.height, pendingReq.index, pendingReq.width, pendingReq.height);
    if (pages.value(req).matchStatus(RequestStatus::Requested)) {
        pages.insert(req, Pair{RequestStatus::Recieved, img});
    }
    if (req==pendingReq) {
        emit pageReady(pages.value(req).img);
        pendingReq = PageRequest();
    }
}
