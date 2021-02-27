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
    pages = QVector<QImage>(backend->maxIndex()+1, QImage()); //tocheck
    pagesStatus = QVector<char>(backend->maxIndex()+1, NOT_REQUESTED);
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
    workerThread.quit();
    workerThread.wait();
}

QImage PageController::getPage() { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
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
    return QImage();
}

void PageController::getAsyncPage() {
    int index = backend->pageIndex();
    if (backend->init() || pagesStatus[index] != RECIEVED)
        initPage(index);
    preloadPages(index);
    emit addPage(pages[index]);
}

void PageController::initPage(int index) {
    ImageWorker w;
    Page p = w.requestImage(backend->bookFilename(), index, backend->width(), backend->height());
    pages[index] = ImageProc::toQImage(p.img).copy();
    pagesStatus[index] = RECIEVED;
    qWarning("initializing %i", index);
}

void PageController::preloadPages(int index) {
    int w = backend->width();
    int h = backend->height();
    int maxIndex = backend->maxIndex();
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (index+i<=maxIndex && pagesStatus[index+i] == NOT_REQUESTED) {
            pagesStatus[index+i] = REQUESTED;
            emit addImage(backend->bookFilename(), index+i, w, h);
        }
        if (index-i>=0 && pagesStatus[index-i] == NOT_REQUESTED) {
            pagesStatus[index-i] = REQUESTED;
            emit addImage(backend->bookFilename(), index-i, w, h);
        }
    }
    for (int i=0; i<=maxIndex; i++) {
        if ((i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD) && pagesStatus[i] == RECIEVED) {
            pages[i] = QImage();
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}

void PageController::handleImage(Page page) {
    if (page.book_filename != backend->bookFilename())
        return;
    qWarning("recieved!!! %i", page.index);
    pages[page.index] = ImageProc::toQImage(page.img).copy();
    pagesStatus[page.index] = RECIEVED;
}

void PageController::changeBookFilename() {
    pages = QVector<QImage>(backend->maxIndex()+1); //tocheck
    pagesStatus = QVector<char>(backend->maxIndex()+1, NOT_REQUESTED);
    lastIndex = backend->pageIndex();
    qWarning("new path: %s", backend->bookFilename().toLocalFile().toStdString().c_str());
}
