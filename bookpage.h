#ifndef BOOKPAGE_H
#define BOOKPAGE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QThread>
#include "book.h"

struct page {
    page* previous=nullptr;
    page* next=nullptr;
    QPixmap* img=nullptr;
    unsigned int length;
};


class BookPage : public QGraphicsPixmapItem
{

public:
    BookPage(QGraphicsItem *parent = nullptr);
    ~BookPage();
    void nextPage();
    void previousPage();


private:
    page *current;
    Book *book;
    std::vector<QPixmap*> pixVect;
    QThread workerThread;

};

#endif // BOOKPAGE_H
