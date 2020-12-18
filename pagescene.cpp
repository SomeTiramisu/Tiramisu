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
    connect(worker, &ImageWorker::imageReady, this, &PageScene::handleImage);
    connect(this, &PageScene::addImage, worker, &ImageWorker::addImage);
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

void PageScene::initPage() {
    QGraphicsPixmapItem *bp = new QGraphicsPixmapItem;
    Image img = Image(book->getCurrent(), book->getLength());

    //could be earlier
    int size = nextItems.size();
    while (size < IMAGE_PRELOAD && nextItemRequest < IMAGE_PRELOAD-size) {
        emit addImage(WIDTH, HEIGHT);
        nextItemRequest++;
    }

    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    addItem(bp);
}

void PageScene::nextPage()
{
    int size = nextItems.size();
    while (size < IMAGE_PRELOAD && nextItemRequest < IMAGE_PRELOAD-size) {
        emit addImage(WIDTH, HEIGHT);
        nextItemRequest++;
    }
    if (!nextItems.empty()) {
        QGraphicsItem* previous = items()[0];
        removeItem(previous);
        delete previous; //seul l'item pas la pixmap!!!

        addItem(nextItems.front());
        nextItems.pop_front();
    }
}

void PageScene::previousPage()
{
    QGraphicsPixmapItem *bp = new QGraphicsPixmapItem;
    Image img = Image(book->getPrevious(), book->getLength());
    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    addItem(bp);
}

void PageScene::handleImage(QPixmap *img) {
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem;
    pixItem->setPixmap(*img);
    nextItems.push_back(pixItem);
    nextItemRequest--;
}
