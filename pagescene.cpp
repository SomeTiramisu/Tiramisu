#include "pagescene.h"

#include "bookpage.h"
#include "image.h"
#include "pageworker.h"
#include <QGraphicsPixmapItem>
#define WIDTH 1200
#define HEIGHT 1920
#define IMAGE_PRELOAD 10


PageScene::PageScene(QObject *parent): QGraphicsScene(parent)
{
    book = new Book();
    pages = new QGraphicsPixmapItem*[book->getSize()];
    for (int i=0; i<book->getSize(); i++) {
        pages[i] = nullptr;
    }

    ImageWorker *worker = new ImageWorker;
    worker->setBook(book);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ImageWorker::imageReady, this, &PageScene::handleImage);
    connect(this, &PageScene::addImage, worker, &ImageWorker::addImage);
    workerThread.start();

    pageIndex = 0;
    leftIndex = pageIndex; //inclus
    rightIndex = pageIndex; //inclus
    maxIndex = book->getSize()-1;
    qWarning("maxIndex %i", maxIndex);
    initPage();

    //addRect(0, 0, 10, 10, QPen(), Qt::red);
    //addRect(1070, 1910, 10, 10, QPen(), Qt::red);
    setBackgroundBrush(Qt::blue);

}

PageScene::~PageScene()
{
    delete book;
    workerThread.quit();
    workerThread.wait();
}

void PageScene::setPage(int index) {
    qWarning("Removing: %i, Showing: %i", pageIndex, index);
    removeItem(pages[pageIndex]);
    pageIndex = index;
    addItem(pages[index]);
}

void PageScene::delPage(int index) {
    qWarning("deleting: %i", index);
    delete pages[index];
    pages[index] = nullptr;

}

void PageScene::initPage() {
    book->setIndex(pageIndex);
    QGraphicsPixmapItem *bp = new QGraphicsPixmapItem;
    Image img = Image(book->getCurrent(), book->getLength());

    //could be earlier
    while(nextItemRequest<IMAGE_PRELOAD && pageIndex+nextItemRequest<maxIndex) {
        nextItemRequest++;
        addImage(pageIndex+nextItemRequest);
    }
    while(previousItemRequest<IMAGE_PRELOAD && pageIndex-previousItemRequest>0) {
        previousItemRequest++;
        addImage(pageIndex-previousItemRequest);
    }

    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    pages[pageIndex] = bp;
    addItem(bp);
}

void PageScene::nextPage()
{
    if (nextItemRequest<IMAGE_PRELOAD && rightIndex+nextItemRequest<maxIndex) {
        nextItemRequest++;
        emit addImage(rightIndex+nextItemRequest);
    }
    if (rightIndex>pageIndex) {

        if (pageIndex-leftIndex>=IMAGE_PRELOAD) { //to check seem correct could lead to adress fault
            delPage(leftIndex);
            leftIndex++;
        }

        setPage(pageIndex+1);
    }
    //qWarning("nextItems: %i", pageIndex);
}

void PageScene::previousPage() {
    if (previousItemRequest<IMAGE_PRELOAD && leftIndex-previousItemRequest>0) {
        previousItemRequest++;
        emit addImage(leftIndex-previousItemRequest);
    }
    if (leftIndex<pageIndex) {

        if (rightIndex-pageIndex>=IMAGE_PRELOAD) { //to check seem correct could lead to adress fault
            delPage(rightIndex);
            rightIndex--;
        }

        setPage(pageIndex-1);
    }
    //qWarning("previousItems: %i", pageIndex);
}

void PageScene::handleImage(QPixmap *img, int index) {
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem;
    pixItem->setPixmap(*img);
    delete img;
    pages[index] = pixItem;
    if (index > pageIndex) {
        rightIndex++;
        nextItemRequest--;
    } else {
        leftIndex--;
        previousItemRequest--;
    }
}

