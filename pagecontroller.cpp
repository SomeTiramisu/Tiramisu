#include "pagecontroller.h"
#include "pageworker.h"
#include "image.h"
#define IMAGE_PRELOAD 5

PageController::PageController(Book* b, QObject *parent) : QObject(parent)
{
    setPageSize(1080, 1920);
    book = b;
    int bookSize = book->getSize();
    pages = new QPixmap*[bookSize];
    pagesStatus = new char[bookSize];
    for (int i = 0; i<bookSize; i++) {
        pages[i] = nullptr;
        pagesStatus[i] = 0;
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

void PageController::setPageSize(int w, int h) {
    pageWidth = w;
    pageHeight = h;
}
QPixmap* PageController::getPage(int index) { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
    requestPages(index);
    if (pagesStatus[index]==0) {
        pagesStatus[index] = 1;
        emit addImage(index, pageWidth, pageHeight);
        return nullptr;
    } else if (pagesStatus[index]==1) {
        return nullptr;
    } else if (pagesStatus[index]==2) {
        return pages[index];
    }
    return nullptr;
}

QPixmap* PageController::initPage(int index) {
    char* buf = book->getAt(index);
    unsigned int length = book->getLength();
    ImageProc img = ImageProc(buf, length);
    delete[] buf;
    try {
        img.process(pageWidth, pageHeight);
        pages[index] = img.toQPixmap();
        pagesStatus[index] = 2;
        return pages[index];
    }  catch (...) {
        qWarning("Something goes wrong with %i", index);
        QPixmap* r = new QPixmap;
        pages[index] = r;
        return r;
    }
}

void PageController::requestPages(int index) {
    int maxIndex = book->getSize()-1;
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (pagesStatus[index+i] == 0 && index+i<=maxIndex) {
            pagesStatus[index+i] = 1;
            emit addImage(index+i, pageWidth, pageHeight);
        }
        if (pagesStatus[index-i] == 0 && index-i>=0) {
            pagesStatus[index-i] = 1;
            emit addImage(index-i, pageWidth, pageHeight);
        }
    }
    for (int i=0; i<=maxIndex; i++) {
        if (pagesStatus[i] == 2 && (i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD)) {
            qWarning("deleting %i", i);
            delete pages[i];
            pages[i] = nullptr;
            pagesStatus[i] = 0;
        }
    }
}

void PageController::handleImage(QPixmap* img, int index) {
    qWarning("page %i revieved!!!", index);
    pages[index] = img;
    pagesStatus[index] = 2;
}

