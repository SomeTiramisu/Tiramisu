#include "pagecontroller.h"
#include "pageworker.h"
#include "imageproc.h"
#include "backend.h"
#include "book.h"
#define IMAGE_PRELOAD 5
#define NOT_REQUESTED 0
#define REQUESTED 1
#define RECIEVED 2

PageController::PageController(Backend* b, QUrl book_filename, QObject *parent) : QObject(parent)
{
    backend = b;
    worker = new ImageWorker(book_filename);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ImageWorker::imageReady, this, &PageController::handleImage);
    connect(this, &PageController::addImage, worker, &ImageWorker::addImage);
    workerThread.start();

    lastIndex = 0;
    this->book_filename = book_filename;
}

PageController::~PageController() {
    workerThread.quit();
    workerThread.wait();
}

QImage PageController::getPage(PageRequest req) { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
    int index = req.index;
    if (pagesStatus[index] != RECIEVED) {
        index = lastIndex;
    }
    int w = req.width;
    int h = req.height;
    preloadPages(req);
    if (pagesStatus[index]==NOT_REQUESTED) {
        pagesStatus[index] = REQUESTED;
        emit addImage(index, w, h);
    } else if (pagesStatus[index]==RECIEVED) {
        lastIndex = index;
        return pages[index];
    }
    qWarning("not recieved i:%i s:%i", index, pagesStatus[index]);
    return QImage();
}

void PageController::getAsyncPage(PageRequest req) {
    int index = req.index;
    if (index >= pages.size()) {
        index = pages.size() - 1;
    }
    if (pagesStatus[index] != RECIEVED)
        initPage(req);
    preloadPages(req);
    emit addPage(pages[index]);
}

void PageController::initPage(PageRequest req) {
    ImageWorker w(book_filename); //shold be the same as bf in request
    Page p = w.requestImage(req.index, req.width, req.height);
    pages[req.index] = ImageProc::toQImage(p.img).copy();
    pagesStatus[req.index] = RECIEVED;
    qWarning("initializing %i", req.index);
}

void PageController::preloadPages(PageRequest req) {
    int index = req.index;
    int w = req.width;
    int h = req.height;
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (index+i<pages.size() && pagesStatus[index+i] == NOT_REQUESTED) {
            pagesStatus[index+i] = REQUESTED;
            emit addImage(index+i, w, h);
        }
        if (index-i>=0 && pagesStatus[index-i] == NOT_REQUESTED) {
            pagesStatus[index-i] = REQUESTED;
            emit addImage(index-i, w, h);
        }
    }
    for (int i=0; i<pages.size(); i++) {
        if ((i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD) && pagesStatus[i] == RECIEVED) {
            pages[i] = QImage();
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}
/*
PageRequest PageController::decodeId(QString id) { //id -> "bookid,index,width,height"
    return PageRequest {
        .width = id.section(",", 2, 2).toInt(),
        .height = id.section(",", 3, 3).toInt(),
        .index = id.section(",", 1, 1).toInt(),
        .book_filename = backend->bookFromId(id.section(",", 0, 0).toInt())
    };
}
*/

void PageController::handleImage(Page page) {
    qWarning("recieved!!! %i", page.index);
    pages[page.index] = ImageProc::toQImage(page.img).copy();
    pagesStatus[page.index] = RECIEVED;
}
