#include "pageimageprovider.h"

#include "image.h"
#include "pageworker.h"
#include <QGraphicsPixmapItem>

#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
//#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"

PageImageProvider::PageImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    backend = new Backend();
    QString fn(ARCHIVE_FILENAME);
    backend->setFilename(fn);
    controller = new PageController(backend->book());
    previousIndex = 0;
    controller->initPage(0);
}

PageImageProvider::~PageImageProvider()
{
    delete backend;
    delete controller;
}
/*
QPixmap PageImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qWarning("requested pageIndex: %i", id.toInt());
    Book *book = backend->book();
    int width = 1080; //TODO: fix to original image size
    int height = 1920;
    int rwidth = requestedSize.width();
    int rheight = requestedSize.height();
    if (size) *size = QSize(width, height);
    book->setIndex(id.toInt());
    ImageProc img = ImageProc(book->getCurrent(), book->getLength());
    img.process(rwidth > 0 ? rwidth : width, rheight > 0 ? rheight : height);
    return *img.toQPixmap();
}
*/
QPixmap PageImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    int width = 1080; //TODO: fix to original image size
    int height = 1920;
    int rwidth = requestedSize.width();
    int rheight = requestedSize.height();
    int rindex = backend->pageIndex();
    qWarning("page %i requested", rindex);
    if (size) *size = QSize(width, height);
    controller->setPageSize(rwidth > 0 ? rwidth : width, rheight > 0 ? rheight : height);
    QPixmap* img(controller->getPage(rindex));
    if (img==nullptr) {
        backend->setPageIndex(previousIndex);
        return *controller->getPage(previousIndex);
    }
    previousIndex = rindex;
    return *img;
}

Backend* PageImageProvider::getBackend() {
    return backend;
}


