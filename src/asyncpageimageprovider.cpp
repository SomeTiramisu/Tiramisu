#include "asyncpageimageprovider.h"

#include "utils/utils.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, PageController *&controller) { //reference sur pointeur
    Q_UNUSED(requestedSize)
    PageRequest req(Utils::decodeId(id));
    if (controller == nullptr) {
        qWarning("bkfn new bkfn: %s", req.book_filename.toString().toStdString().c_str());
        controller = new PageController(req.book_filename);
    }
    if (controller->getBookFilename() != req.book_filename) {
        qWarning("bkfn changed old: %s, new: %s", controller->getBookFilename().toString().toStdString().c_str(), req.book_filename.toString().toStdString().c_str());
        controller->deleteLater();
        controller = new PageController(req.book_filename);
    }
    connect(controller, &PageController::pageReady, this, &AsyncPageImageResponse::handleDone);
    controller->getAsyncPage(req);
}
void AsyncPageImageResponse::handleDone(QImage img) {
    m_image = img;
    //qWarning("finished");
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    qWarning("Provider: sending %i %i", m_image.width(), m_image.height());
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncPageImageProvider::AsyncPageImageProvider()
    : QQuickAsyncImageProvider(),
      controller(nullptr) {
}

QQuickImageResponse *AsyncPageImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, controller);
    return response;
}
