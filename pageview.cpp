#include "pageview.h"
#include "pagescene.h"
#include <QMouseEvent>

PageView::PageView(QWidget *parent)
    :QGraphicsView(parent)
{
    setStyleSheet("border: 0px");
    pagescene = new PageScene(width(), height(), this);
    setScene(pagescene);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setSceneRect(0, 0, width(), height());

}

PageView::~PageView() {
    delete pagescene;
}


void PageView::resizeEvent(QResizeEvent *event) {
    QGraphicsItem *px = scene()->items()[0];

}

void PageView::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x();
    int m = width() / 2;
    if (x > m) { pagescene->nextPage(); }
    else { pagescene->previousPage(); };

}


