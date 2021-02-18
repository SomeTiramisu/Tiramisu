#include "pageimageprovider.h"

#include "image.h"
#include "pageworker.h"
#include <QGraphicsPixmapItem>

PageImageProvider::PageImageProvider(Backend *b)
    : QQuickImageProvider(QQuickImageProvider::Pixmap),
      controller(b)
{
}

PageImageProvider::~PageImageProvider()
{
}

QPixmap PageImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(id)
    QPixmap* img(controller.getPage());
    if (img == nullptr)
        return QPixmap();
    qWarning("showing page");
    int rwidth = requestedSize.width();
    int rheight = requestedSize.height();
    if (size)
        *size = QSize(img->width(), img->height());
    if (rwidth > 0 || rheight > 0)
        return img->copy(0, 0, rwidth, rheight);
    return *img;
}
