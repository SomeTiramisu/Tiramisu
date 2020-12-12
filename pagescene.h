#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>
#include "bookpage.h"

class PageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PageScene(QObject *parent = nullptr);
    ~PageScene();
    void nextPage();
    void previousPage();
private:
    QGraphicsPixmapItem *bp;
    Book *book;
};

#endif // PAGESCENE_H
