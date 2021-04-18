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

{
    int book_size = book.getSize();
    pages = QVector<Pair>(book_size, Pair()); //tocheck
}

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
    if (pages[index].matchRequest(req) && pages[index].matchStatus(RequestStatus::Recieved)) {
        emit pageReady(pages[index].img);
    } else if (pages[index].matchRequest(req) && pages[index].matchStatus(RequestStatus::Requested)) {
        pendingReq = req;
    } else {
        pendingReq = req;
        runPage(req, PRIORITY_MAX);
    }
    preloadPages(req);
}

void PageController::preloadPages(PageRequest req) {
    int index = req.index;
    int book_size = book.getSize();
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (index+i<book_size && (pages[index+i].matchStatus(RequestStatus::NotRequested) || not pages[index+i].matchRequest(req.addIndex(i)))) {
            PageRequest new_req(req);
            new_req.index += i;
            runPage(new_req, PRIORITY_REQ);
        }
        if (index-i>=0 && (pages[index-i].matchStatus(RequestStatus::NotRequested) || not pages[index-i].matchRequest(req.addIndex(-i)))) {
            PageRequest new_req(req);
            new_req.index -= i;
            runPage(new_req, PRIORITY_REQ);
        }
    }
    for (int i=0; i<book_size; i++) {
        if ((i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD) && pages[i].matchStatus(RequestStatus::Recieved)) {
            pages[i] = Pair();
        }
    }
}

void PageController::runPage(PageRequest req, int priority) {
    pages[req.index].status = RequestStatus::Requested;
    pages[req.index].req = req;
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
    if (pages[req.index].matchStatus(RequestStatus::Requested) && pages[req.index].matchRequest(req)) {
        pages[req.index] = Pair{req, RequestStatus::Recieved, img.copy()};
    }
    if (req==pendingReq) {
        emit pageReady(pages[req.index].img);
        pendingReq = PageRequest();
    }
}
