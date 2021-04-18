#include "pagecontroller.h"
#include "imagerunnable.h"
#include "utils/imageproc.h"
#include "parsers/parser.h"
#define IMAGE_PRELOAD 10
#define NOT_REQUESTED 0
#define REQUESTED 1
#define RECIEVED 2
#define PRIORITY_MAX 0
#define PRIORITY_REQ 1

PageController::PageController(QUrl book_filename, QObject *parent)
    : QObject(parent),
      book(book_filename, true)

{
    int book_size = book.getSize();
    pages = QVector<PageResponseQ>(book_size, PageResponseQ()); //tocheck
    pagesStatus = QVector<char>(book_size, NOT_REQUESTED);
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
    if (index >= book_size) {
        index = book_size - 1;
    }
    if (pagesStatus[index] == RECIEVED && pages[index] == req) {
        emit pageReady(pages[index]);
    } else if (pagesStatus[index] == REQUESTED && pages[index] == req) {
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
        if (index+i<book_size && (pagesStatus[index+i] == NOT_REQUESTED/* || pages[index+i] != req.addIndex(i)*/)) {
            PageRequest new_req(req);
            new_req.index += i;
            runPage(new_req, PRIORITY_REQ);
        }
        if (index-i>=0 && (pagesStatus[index-i] == NOT_REQUESTED/* || pages[index-i] != req.addIndex(-i)*/)) {
            PageRequest new_req(req);
            new_req.index -= i;
            runPage(new_req, PRIORITY_REQ);
        }
    }
    for (int i=0; i<book_size; i++) {
        if ((i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD) && pagesStatus[i] == RECIEVED) {
            pages[i] = PageResponseQ();
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}

void PageController::runPage(PageRequest req, int priority) {
    pagesStatus[req.index] = REQUESTED;
    pages[req.index] = {req, QImage()};
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

void PageController::handleImage(PageResponseCV resp) {
    qWarning("recieved!!! %i %i %i pending: %i %i %i", resp.index, resp.width, resp.height, pendingReq.index, pendingReq.width, pendingReq.height);
    pages[resp.index] = {resp, ImageProc::toQImage(resp.img).copy()};
    pagesStatus[resp.index] = RECIEVED;
    if (resp==pendingReq) {
        emit pageReady(pages[resp.index]);
        pendingReq = PageRequest();
    }
}
