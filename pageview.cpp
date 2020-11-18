#include "pageview.h"
#include "pagescene.h"
#include <QMouseEvent>

PageView::PageView(QWidget *parent)
    :QGraphicsView(parent)
{
    setStyleSheet("border: 0px");
    pagescene = new PageScene(this);
    setScene(pagescene);
    setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    //setRenderHint(QPainter::Antialiasing);
}

PageView::~PageView() {
    delete pagescene;
}


void PageView::resizeEvent(QResizeEvent *event) {
    QGraphicsItem *px = scene()->items()[0];
    fitInView(px, Qt::KeepAspectRatio);
}

void PageView::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x();
    int m = width() / 2;
    if (x > m) { pagescene->nextPage(); }
    else { pagescene->previousPage(); };

}


