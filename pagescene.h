#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>
#include <deque>
#include "bookpage.h"
#include "image.h"

class PageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PageScene(QObject *parent = nullptr);
    ~PageScene();
    void initPage();
    void nextPage();
    void previousPage();
private:
    std::deque<QGraphicsPixmapItem*> nextItems;
    std::deque<QGraphicsPixmapItem*> previousItems;
    QGraphicsPixmapItem* currentItem;
    int nextItemRequest = 0;
    int previousItemRequest = 0;
    Book *book;
    QThread workerThread;

public slots:
    void handleImage(QPixmap *img);
signals:
    void addImage(double width, double height);
};

#endif // PAGESCENE_H
