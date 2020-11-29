#include "pagescene.h"
#include "bookpage.h"

PageScene::PageScene(QObject *parent): QGraphicsScene(parent)
{
    /*
    QPixmap p;
    QImage i;
    i.load("/home/guillaume/reader/000.jpg");
    p.convertFromImage(i);
    addPixmap(p);
    */
    bp = new BookPage;
    addItem(bp);
    //addRect(0, 0, 10, 10, QPen(), Qt::red);
    //addRect(1070, 1910, 10, 10, QPen(), Qt::red);
    setBackgroundBrush(Qt::blue);

}

PageScene::~PageScene()
{
    delete bp;
}

void PageScene::nextPage()
{
    bp->nextPage();
}

void PageScene::previousPage()
{
    bp->previousPage();
}
