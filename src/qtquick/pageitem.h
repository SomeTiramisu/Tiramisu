#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QQuickPaintedItem>
#include <QTimer>
#include <QImage>
#include "../libtiramisu/tiramisu.h"

class PageItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int bookSize READ bookSize NOTIFY bookSizeChanged)
    Q_PROPERTY(int preloaderProgress READ preloaderProgress NOTIFY preloaderProgressChanged)

public:
    PageItem(QQuickItem *parent = nullptr);
    ~PageItem() override;
    QUrl filename() const {return m_filename;};
    int index() const {return m_index;};
    int bookSize() const {return m_tiramisu.bookSize();}
    int preloaderProgress() const { return m_tiramisu.preloaderProgress();}
    void setFilename(const QUrl& filename);
    void setIndex(int index);
    void paint(QPainter *painter) override;

private:
    QUrl m_filename;
    int m_index{0};
    Tiramisu m_tiramisu;
    PageRequest m_req;
    QImage m_image;
    QImage m_tmpImage; //funny things appened if we ever resize m_image
    QTimer m_resizeTimer;

signals:
    void filenameChanged();
    void indexChanged();
    void bookSizeChanged();
    void preloaderProgressChanged();
    void imageChanged();

public slots:
    void onRotationChanged();
    void resizeTimeout();
    void onImageChanged();
};

QImage toQImage(const cv::Mat& src);

#endif // PAGEITEM_H
