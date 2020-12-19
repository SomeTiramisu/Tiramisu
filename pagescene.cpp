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

    ImageWorker *worker = new ImageWorker;
    worker->setBook(book);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ImageWorker::nextImageReady, this, &PageScene::handleNextImage);
    connect(worker, &ImageWorker::previousImageReady, this, &PageScene::handlePreviousImage);
    connect(this, &PageScene::addNextImage, worker, &ImageWorker::addNextImage);
    connect(this, &PageScene::addPreviousImage, worker, &ImageWorker::addPreviousImage);
    workerThread.start();

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

QGraphicsPixmapItem* PageScene::setPage(QGraphicsPixmapItem *bp) {
    removeItem(currentItem);
    QGraphicsPixmapItem* oldItem = currentItem;
    currentItem = bp;
    addItem(bp);
    return oldItem;
}

void PageScene::initPage() {
    QGraphicsPixmapItem *bp = new QGraphicsPixmapItem;
    Image img = Image(book->getCurrent(), book->getLength());

    //could be earlier
    int size = nextItems.size();
    while (size < IMAGE_PRELOAD && nextItemRequest < IMAGE_PRELOAD-size) {
        emit addNextImage();
        nextItemRequest++;
    }

    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    currentItem = bp;
    addItem(bp);
}

void PageScene::nextPage()
{
    int size = nextItems.size();
    while (size < IMAGE_PRELOAD && nextItemRequest < IMAGE_PRELOAD-size) {
        emit addNextImage();
        nextItemRequest++;
    }
    if (!nextItems.empty()) {
        QGraphicsPixmapItem* oldItem = setPage(nextItems.front());
        previousItems.push_front(oldItem);
        nextItems.pop_front();
        if (previousItems.size() > IMAGE_PRELOAD) {
            delete previousItems.back();
            previousItems.pop_back();
        }
    }
    qWarning("nextItems: %i", nextItems.size());
}


void PageScene::previousPage()
{
    int size = previousItems.size();
    while (size < IMAGE_PRELOAD && previousItemRequest < IMAGE_PRELOAD-size) {
        emit addPreviousImage();
        previousItemRequest++;
    }
    if (!previousItems.empty()) {
        QGraphicsPixmapItem* oldItem = setPage(previousItems.front());
        nextItems.push_front(oldItem);
        previousItems.pop_front();
        if (nextItems.size() > IMAGE_PRELOAD) {
            delete nextItems.back();
            nextItems.pop_back();
        }
    }
    qWarning("previousItems: %i", previousItems.size());
}

void PageScene::handleNextImage(QPixmap *img) {
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem;
    pixItem->setPixmap(*img);
    nextItems.push_back(pixItem);
    nextItemRequest--;
}

void PageScene::handlePreviousImage(QPixmap *img) {
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem;
    pixItem->setPixmap(*img);
    previousItems.push_back(pixItem);
    previousItemRequest--;
}
