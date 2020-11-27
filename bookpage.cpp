#include "bookpage.h"
#include "book.h"
#include <QByteArray>
#include <image.h>

BookPage::BookPage()
{
    book = new Book();
    current = new page;
    Image img = Image(book->getCurrent(), book->getLength());

    img.process(1200, 1920);
    current->img = img.toQPixmap();

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
        Image img = Image(book->getNext(), book->getLength());
        img.process(1200, 1920);
        current->img = img.toQPixmap();
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
