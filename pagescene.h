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
    PageScene(int w, int h, QObject *parent = nullptr);
    ~PageScene();
    void initPage();
    void nextPage();
    void previousPage();
    void setPageSize(int w, int h);
private:
    int pageWidth = 0;
    int pageHeight = 0;
    void setPage(int index);
    void delPage(int index);
    int nextItemRequest = 0;
    int previousItemRequest = 0;
    Book *book;
    QThread workerThread;
    int pageIndex;
    int leftIndex;
    int rightIndex;
    int maxIndex;
    QGraphicsPixmapItem** pages = new QGraphicsPixmapItem*[0];

public slots:
    void handleImage(QPixmap *img, int index);
signals:
    void addImage(int index);
};

#endif // PAGESCENE_H
