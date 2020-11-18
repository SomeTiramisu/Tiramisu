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
    BookPage *bp;
};

#endif // PAGESCENE_H
