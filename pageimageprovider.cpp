#include "pageimageprovider.h"

#include "image.h"
#include "pageworker.h"
#include <QGraphicsPixmapItem>

//#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"

PageImageProvider::PageImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    backend = new Backend();
    QString fn(ARCHIVE_FILENAME);
    backend->setFilename(fn);
    controller = new PageController(backend->book());
    previousIndex = 0;
}

PageImageProvider::~PageImageProvider()
{
    delete backend;
    delete controller;
}

QPixmap PageImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    int width = 500; //TODO: fix to original image size
    int height = 500;
    int rwidth = requestedSize.width();
    int rheight = requestedSize.height();
    int rindex = backend->pageIndex();
    int fwidth = rwidth > 0 ? rwidth : width;
    int fheight = rheight > 0 ? rheight : height;
    if (size) *size = QSize(width, height);
    if (id == "first") controller->initPage(rindex, fwidth, fheight);
    QPixmap* img(controller->getPage(rindex, fwidth, fheight));
    if (img==nullptr) {
        backend->setPageIndex(previousIndex);
        qWarning("showing %i", previousIndex);
        return *controller->getPage(previousIndex, 100, 100); //TODO return correct size
    }
    previousIndex = rindex;
    qWarning("showing %i", rindex);
    return *img;
}

Backend* PageImageProvider::getBackend() {
    return backend;
}


