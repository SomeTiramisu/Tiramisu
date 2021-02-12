#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include "book.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(Book *b, QObject *parent = nullptr);
    ~PageController();
    QPixmap* getPage(int index, int w, int h);
    QPixmap* initPage(int index, int w, int h);


private:
    void preloadPages(int index, int w, int h);
    Book *book;
    QThread workerThread;
    QPixmap* *pages;
    char *pagesStatus;

public slots:
    void handleImage(QPixmap* img, int index);
signals:
    void addImage(int index, int width, int height);

};

#endif // PAGECONTROLLER_H
