#include "pagecontroller.h"
#include "pageworker.h"
#include "imageproc.h"
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

    connect(backend, &Backend::bookFilenameChanged, this, &PageController::changeBookFilename);

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
    if (backend->init()) //aucune page n'a encre ete demandees
        initPage(index);
    if (pagesStatus[index] != RECIEVED) {
        index = lastIndex;
        backend->setPageIndex(lastIndex);
    }
    int w = backend->width();
    int h = backend->height();
    preloadPages(index);
    if (pagesStatus[index]==NOT_REQUESTED) {
        pagesStatus[index] = REQUESTED;
        emit addImage(backend->bookFilename(), index, w, h);
    } else if (pagesStatus[index]==RECIEVED) {
        lastIndex = index;
        return pages[index];
    }
    qWarning("not recieved i:%i s:%i", index, pagesStatus[index]);
    return nullptr;
}

void PageController::initPage(int index) {
    ImageWorker w;
    Page p = w.requestImage(backend->bookFilename(), index, backend->width(), backend->height());
    pages[index] = ImageProc::toQPixmap(p.img);
    pagesStatus[index] = RECIEVED;
    qWarning("initializing %i", index);
}

void PageController::preloadPages(int index) {
    int w = backend->width();
    int h = backend->height();
    int maxIndex = backend->maxIndex();
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (pagesStatus[index+i] == NOT_REQUESTED && index+i<=maxIndex) {
            pagesStatus[index+i] = REQUESTED;
            emit addImage(backend->bookFilename(), index+i, w, h);
        }
        if (pagesStatus[index-i] == NOT_REQUESTED && index-i>=0) {
            pagesStatus[index-i] = REQUESTED;
            emit addImage(backend->bookFilename(), index-i, w, h);
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

void PageController::handleImage(Page page) {
    if (page.book_filename != backend->bookFilename())
        return;
    qWarning("recieved!!! %i", page.index);
    if (pagesStatus[page.index] == RECIEVED) {
        delete pages[page.index];
    }
    pages[page.index] = ImageProc::toQPixmap(page.img);
    pagesStatus[page.index] = RECIEVED;
}

void PageController::changeBookFilename() {
    delete[] pages;
    delete[] pagesStatus;
    pages = new QPixmap*[backend->maxIndex()+1]; //tocheck
    pagesStatus = new char[backend->maxIndex()+1];
    for (int i = 0; i<=backend->maxIndex(); i++) {
        pages[i] = nullptr;
        pagesStatus[i] = NOT_REQUESTED;
    }
    lastIndex = backend->pageIndex();
    qWarning("new path: %s", backend->bookFilename().toLocalFile().toStdString().c_str());
}
