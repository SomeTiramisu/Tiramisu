#include <asyncpageimageprovider.h>
#include <QImage>
//#include "pageworker.h"
//#include "imageproc.h"
//#include "backend.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, PageController *&controller) {
    PageRequest req(decodeId(id));
    if (controller == nullptr) {
        qWarning("bkfn new bkfn: %s", req.book_filename.toString().toStdString().c_str());
        controller = new PageController(req.book_filename);
    }
    if (controller->getBookFilename() != req.book_filename) {
        qWarning("bkfn changed old: %s, new: %s", controller->getBookFilename().toString().toStdString().c_str(), req.book_filename.toString().toStdString().c_str());
        delete controller;
        controller = new PageController(req.book_filename);
    }
    connect(controller, &PageController::addPage, this, &AsyncPageImageResponse::handleDone);
    controller->getAsyncPage(req);
}
void AsyncPageImageResponse::handleDone(QImage image) {
    m_image = image;
    //qWarning("finished");
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    qWarning("sending %i %i", m_image.width(), m_image.height());
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncPageImageProvider::AsyncPageImageProvider()
    : QQuickAsyncImageProvider() {
    controller = nullptr;
}

QQuickImageResponse *AsyncPageImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, controller);
    return response;
}