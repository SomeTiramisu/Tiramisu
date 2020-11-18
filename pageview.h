#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>
#include "pagescene.h"

class PageView : public QGraphicsView
{
    Q_OBJECT
public:
    PageView(QWidget *parent = nullptr);
    ~PageView();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    PageScene* pagescene;

};

#endif // PAGEVIEW_H
