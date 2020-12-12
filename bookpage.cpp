#include "bookpage.h"
#include "book.h"
#include <QByteArray>
#include <QObject>
#include <image.h>

#define WIDTH 1080
#define HEIGHT 1920

BookPage::BookPage(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    Image img = Image(book->getCurrent(), book->getLength());
    img.process(WIDTH, HEIGHT);
    setPixmap(*img.toQPixmap());

}

BookPage::~BookPage()
{
    delete book;
    delete current;
}

void BookPage::nextPage()
{
    Image img = Image(book->getNext(), book->getLength());
    img.process(WIDTH, HEIGHT);
    setPixmap(*img.toQPixmap());
}

void BookPage::previousPage()
{
    Image img = Image(book->getPrevious(), book->getLength());
    img.process(WIDTH, HEIGHT);
    setPixmap(*img.toQPixmap());
}
