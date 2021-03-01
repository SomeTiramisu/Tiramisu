#include <asyncpageimageprovider.h>
#include <QImage>
#include "pageworker.h"
#include "imageproc.h"
#include "backend.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, PageController &controller) {
    connect(&controller, &PageController::addPage, this, &AsyncPageImageResponse::handleDone);
    controller.getAsyncPage(id);
}
void AsyncPageImageResponse::handleDone(QImage image) {
    m_image = image;
    qWarning("finished");
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    qWarning("sending %i %i", m_image.width(), m_image.height());
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncPageImageProvider::AsyncPageImageProvider(Backend *b)
    : QQuickAsyncImageProvider(), controller(b) {
}

QQuickImageResponse *AsyncPageImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, controller);
    return response;
}
