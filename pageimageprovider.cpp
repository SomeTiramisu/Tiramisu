#include "pageimageprovider.h"

#include "imageproc.h"
#include "pageworker.h"
#include <QGraphicsPixmapItem>

PageImageProvider::PageImageProvider(Backend *b)
    : QQuickImageProvider(QQuickImageProvider::Image),
      controller(b)
{
}

PageImageProvider::~PageImageProvider()
{
}

QImage PageImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QImage img(controller.getPage(id));
    if (img.isNull())
        return QImage();
    //qWarning("showing page");
    int rwidth = requestedSize.width();
    int rheight = requestedSize.height();
    if (size)
        *size = QSize(img.width(), img.height());
    if (rwidth > 0 || rheight > 0)
        return img.copy(0, 0, rwidth, rheight);
    return img;
}
