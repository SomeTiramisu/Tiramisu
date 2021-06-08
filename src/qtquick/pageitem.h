#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QQuickPaintedItem>
#include <QTimer>
#include "pagescheduler.h"

class PageItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int bookSize READ bookSize NOTIFY bookSizeChanged)

public:
    PageItem(QQuickItem *parent = nullptr);
    ~PageItem() override;
    QUrl filename() const {return m_filename;};
    int index() const {return m_index;};
    int bookSize() const {return m_bookSize;}
    void setFilename(const QUrl& filename);
    void setIndex(int index);
    void paint(QPainter *painter) override;

private:
    QUrl m_filename;
    int m_index{0};
    int m_bookSize{0};
    PageScheduler* m_scheduler{nullptr};
    PagePreloader* m_preloader{nullptr};
    PageRequest m_req;
    QImage m_image;
    QTimer m_resizeTimer;

signals:
    void filenameChanged();
    void indexChanged();
    void bookSizeChanged();

public slots:
    void onRotationChanged();
    void resizeTimeout();
    void handleImage(PageRequest req, QImage img);
};

#endif // PAGEITEM_H
