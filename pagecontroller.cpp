#include "pagecontroller.h"
#include "imagerunnable.h"
#include "imageproc.h"
#include "book.h"
#define IMAGE_PRELOAD 10
#define NOT_REQUESTED 0
#define REQUESTED 1
#define RECIEVED 2

PageController::PageController(QUrl book_filename, QObject *parent) :
    QObject(parent),
    book(book_filename),
    book_filename(book_filename)
{
    book_size = book.getSize();
    pages = QVector<QImage>(book_size, QImage()); //tocheck
    pagesStatus = QVector<char>(book_size, NOT_REQUESTED);

    lastIndex = 0;


}

PageController::~PageController() {

    qWarning("controller deleted");
}

QImage PageController::getPage(PageRequest req) { //0 -> no requested no revieved ; 1 -> requested no recieved ; 2 -> recieved
    int index = req.index;
    if (pagesStatus[index] != RECIEVED) {
        index = lastIndex;
    }
    preloadPages(req);
    if (pagesStatus[index]==NOT_REQUESTED) {
        pagesStatus[index] = REQUESTED;
        runLocalPage(req);
    } else if (pagesStatus[index]==RECIEVED) {
        lastIndex = index;
        return pages[index];
    }
    //qWarning("not recieved i:%i s:%i", index, pagesStatus[index]);
    return QImage();
}

void PageController::getAsyncPage(PageRequest req) {
    int index = req.index;
    if (index >= book_size) {
        index = book_size - 1;
    }
    //preloadPages(req);
    if (pagesStatus[index] != RECIEVED)
        runLocalPage(req);
    emit addPage(pages[index]);
    preloadPages(req);
}

void PageController::preloadPages(PageRequest req) {
    int index = req.index;
    for (int i=1; i<IMAGE_PRELOAD; i++) {
        if (index+i<book_size && pagesStatus[index+i] == NOT_REQUESTED) {
            pagesStatus[index+i] = REQUESTED;
            PageRequest new_req(req);
            new_req.index += i;
            runPage(new_req);
        }
        if (index-i>=0 && pagesStatus[index-i] == NOT_REQUESTED) {
            pagesStatus[index-i] = REQUESTED;
            PageRequest new_req(req);
            new_req.index -= i;
            runPage(new_req);
        }
    }
    for (int i=0; i<book_size; i++) {
        if ((i < index - IMAGE_PRELOAD || i > index + IMAGE_PRELOAD) && pagesStatus[i] == RECIEVED) {
            pages[i] = QImage();
            pagesStatus[i] = NOT_REQUESTED;
        }
    }
}

void PageController::runPage(PageRequest req) {
    ImageRunnable *runnable = new ImageRunnable(book, req);
    connect(runnable, &ImageRunnable::done, this, &PageController::handleImage);
    pool.start(runnable);
}

void PageController::runLocalPage(PageRequest req) {
    ImageRunnable *runnable = new ImageRunnable(book, req);
    connect(runnable, &ImageRunnable::done, this, &PageController::handleImage);
    runnable->run();
    delete runnable;
}

QUrl PageController::getBookFilename() {
    return book_filename;
}

void PageController::handleImage(Page page) {
    qWarning("recieved!!! %i", page.index);
    pages[page.index] = ImageProc::toQImage(page.img).copy();
    pagesStatus[page.index] = RECIEVED;
}
