#include "asyncpageimageprovider.h"

#include "utils/utils.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QHash<QString, PageController*>& controllers) { //reference sur pointeur
    Q_UNUSED(requestedSize)
    PageRequest req(Utils::decodeId(id));
    PageController* controller = controllers.value(req.controller_id);
    if (controller == nullptr) {
        controller = new PageController(req.book_filename);
        controllers.insert(req.controller_id, controller);
    } else if (controller->getBookFilename() != req.book_filename) {
        controller->deleteLater();
        controller = new PageController(req.book_filename);
        controllers.insert(req.controller_id, controller);
    }
    connect(controller, &PageController::pageReady, this, &AsyncPageImageResponse::handleDone);
    controller->getAsyncPage(req);
}
void AsyncPageImageResponse::handleDone(QImage img) {
    m_image = img;
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    qWarning("Provider: sending %i %i", m_image.width(), m_image.height());
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncPageImageProvider::AsyncPageImageProvider()
    : QQuickAsyncImageProvider()
{}

AsyncPageImageProvider::~AsyncPageImageProvider() {
}

QQuickImageResponse *AsyncPageImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, controllers);
    return response;
}
