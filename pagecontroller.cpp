#include "pagecontroller.h"
#include "pageworker.h"
#include "image.h"
#include "backend.h"
#define IMAGE_PRELOAD 5
#define NOT_REQUESTED 0
#define REQUESTED 1
#define RECIEVED 2

PageController::PageController(Backend* b, QObject *parent) : QObject(parent)
{
    backend = b;
    pages = new QPixmap*[backend->maxIndex()+1]; //tocheck
    pagesStatus = new char[backend->maxIndex()+1];
    for (int i = 0; i<=backend->maxIndex(); i++) {
        pages[i] = nullptr;
        pagesStatus[i] = NOT_REQUESTED;
    }
    worker = new ImageWorker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ImageWorker::imageReady, this, &PageController::handleImage);
    connect(this, &PageController::addImage, worker, &ImageWorker::addImage);
    workerThread.start();

    initPage(backend->pageIndex());
    lastIndex = backend->pageIndex();
}

PageController::~PageController() {
    delete pages;
    delete pagesStatus;
    workerThread.quit();
    workerThread.wait();
}

QPixmap* PageController::getPage() { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
    int index = backend->pageIndex();
    if (pagesStatus[index] != RECIEVED) {
        index = lastIndex;
        backend->setPageIndex(lastIndex);
    }
    int w = backend->width();
    int h = backend->height();
    preloadPages(index);
    if (pagesStatus[index]==NOT_REQUESTED) {
        pagesStatus[index] = REQUESTED;
        emit addImage(backend->bookFilename(), backend->bgFilename(), index, w, h);
    } else if (pagesStatus[index]==RECIEVED) {
        lastIndex = index;
        return pages[index];
    }
    qWarning("not recieved i:%i s:%i", index, pagesStatus[index]);
    return nullptr;
}

void PageController::initPage(int index) {
    pages[index] = worker->requestImage(backend->bookFilename(), backend->bgFilename(), index, backend->width(), backend->height());
    pagesStatus[index] = RECIEVED;
}

void PageController::preloadPages(int index) {
    int w = backend->width();
    int h = backend->height();
    int maxIndex = backend->maxIndex();
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (pagesStatus[index+i] == NOT_REQUESTED && index+i<=maxIndex) {
            pagesStatus[index+i] = REQUESTED;
            emit addImage(backend->bookFilename(), backend->bgFilename(), index+i, w, h);
        }
        if (pagesStatus[index-i] == NOT_REQUESTED && index-i>=0) {
            pagesStatus[index-i] = REQUESTED;
            emit addImage(backend->bookFilename(), backend->bgFilename(), index-i, w, h);
        }
    }
    for (int i=0; i<=maxIndex; i++) {
        if (pagesStatus[i] == RECIEVED && (i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD)) {
            //qWarning("deleting %i", i);
            delete pages[i];
            pages[i] = nullptr;
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}

void PageController::handleImage(QPixmap* img, int index) {
    qWarning("recieved!!! %i", index);
    if (pagesStatus[index] == RECIEVED) {
        delete pages[index];
    }
    pages[index] = img;
    pagesStatus[index] = RECIEVED;
}

