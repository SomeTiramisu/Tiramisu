#include "pagescene.h"
#include "bookpage.h"

PageScene::PageScene(QObject *parent)
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
