#include "pagecontroller.h"
#include "pageworker.h"
#include "image.h"
#define IMAGE_PRELOAD 5
#define NOT_REQUESTED 0
#define REQUESTED 1
#define RECIEVED 2

PageController::PageController(Book* b, QObject *parent) : QObject(parent)
{
    book = b;
    int bookSize = book->getSize();
    pages = new QPixmap*[bookSize];
    pagesStatus = new char[bookSize];
    for (int i = 0; i<bookSize; i++) {
        pages[i] = nullptr;
        pagesStatus[i] = NOT_REQUESTED;
    }
    ImageWorker *worker = new ImageWorker;
    worker->setBook(b);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ImageWorker::imageReady, this, &PageController::handleImage);
    connect(this, &PageController::addImage, worker, &ImageWorker::addImage);
    workerThread.start();
}

PageController::~PageController() {
    delete pages;
    delete pagesStatus;
    workerThread.quit();
    workerThread.wait();
}

QPixmap* PageController::getPage(int index, int w, int h) { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
    preloadPages(index, w, h);
    if (pagesStatus[index]==NOT_REQUESTED) {
        pagesStatus[index] = REQUESTED;
        emit addImage(index, w, h);
        return nullptr;
    } else if (pagesStatus[index]==REQUESTED) {
        return nullptr;
    } else if (pagesStatus[index]==RECIEVED) {
        return pages[index];
    }
    return nullptr;
}

QPixmap* PageController::initPage(int index, int w, int h) {
    char* buf = book->getAt(index);
    long long length = book->getLength(index);
    ImageProc img = ImageProc(buf, length);
    delete[] buf;
    try {
        img.process(w, h);
        pages[index] = img.toQPixmap();
        pagesStatus[index] = RECIEVED;
        return pages[index];
    }  catch (...) {
        qWarning("Something goes wrong with %i", index);
        QPixmap* r = new QPixmap;
        pages[index] = r;
        return r;
    }
}

void PageController::preloadPages(int index, int w, int h) {
    int maxIndex = book->getSize()-1;
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (pagesStatus[index+i] == NOT_REQUESTED && index+i<=maxIndex) {
            pagesStatus[index+i] = REQUESTED;
            emit addImage(index+i, w, h);
        }
        if (pagesStatus[index-i] == NOT_REQUESTED && index-i>=0) {
            pagesStatus[index-i] = REQUESTED;
            emit addImage(index-i, w, h);
        }
    }
    for (int i=0; i<=maxIndex; i++) {
        if (pagesStatus[i] == RECIEVED && (i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD)) {
            qWarning("deleting %i", i);
            delete pages[i];
            pages[i] = nullptr;
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}

void PageController::handleImage(QPixmap* img, int index) {
    //qWarning("recieved!!! %i", index);
    pages[index] = img;
    pagesStatus[index] = RECIEVED;
}

