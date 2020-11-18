#include "bookpage.h"
#include "book.h"
#include <QByteArray>

BookPage::BookPage()
{
    book = new Book();
    current = new page;
    current->img = new QPixmap();
    current->img->loadFromData(QByteArray(book->getCurrent(), book->getLength()));
    setPixmap(*current->img);

}

BookPage::~BookPage()
{
    delete book;
    delete current;
}

void BookPage::nextPage()
{
    if (current->next == nullptr) {
        current->next = new page;
        current->next->previous = current;
        current = current->next;
        current->img = new QPixmap();
        current->img->loadFromData(QByteArray(book->getNext(), book->getLength()));
    } else {
        current = current->next;
    }
    setPixmap(*current->img);
}

void BookPage::previousPage()
{
    if (current->previous != nullptr) {
        current = current->previous;
        setPixmap(*current->img);
    }
}
