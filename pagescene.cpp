#include "pagescene.h"

#include "bookpage.h"
#include "image.h"
#include <QGraphicsPixmapItem>
#define WIDTH 1080
#define HEIGHT 1920

PageScene::PageScene(QObject *parent): QGraphicsScene(parent)
{
    /*
    QPixmap p;
    QImage i;
    i.load("/home/guillaume/reader/000.jpg");
    p.convertFromImage(i);
    addPixmap(p);
    */
    book = new Book();

    bp = new QGraphicsPixmapItem;
    Image img = Image(book->getCurrent(), book->getLength());
    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());

    addItem(bp);
    //addRect(0, 0, 10, 10, QPen(), Qt::red);
    //addRect(1070, 1910, 10, 10, QPen(), Qt::red);
    setBackgroundBrush(Qt::blue);

}

PageScene::~PageScene()
{
    delete bp;
    delete book;
}

void PageScene::nextPage()
{
    bp = new QGraphicsPixmapItem;
    Image img = Image(book->getNext(), book->getLength());
    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    addItem(bp);
}

void PageScene::previousPage()
{
    bp = new QGraphicsPixmapItem;
    Image img = Image(book->getPrevious(), book->getLength());
    img.process(WIDTH, HEIGHT);
    bp->setPixmap(*img.toQPixmap());
    addItem(bp);
}
