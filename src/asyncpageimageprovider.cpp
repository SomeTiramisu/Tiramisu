#include "asyncpageimageprovider.h"

#include "utils/utils.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QHash<QString, PageController*>& controllers) //reference sur pointeur
    : m_req(Utils::decodeId(id))
{
    Q_UNUSED(requestedSize)
    PageController* controller = controllers.value(m_req.controller_id());
    if (controller == nullptr) {
        controller = new PageController(m_req.book_filename(), true, m_req.controller_preload());
        controllers.insert(m_req.controller_id(), controller);
    } else if (controller->getBookFilename() != m_req.book_filename()) {
        controller->deleteLater();
        controller = new PageController(m_req.book_filename(), true, m_req.controller_preload());
        controllers.remove(m_req.controller_id());
        controllers.insert(m_req.controller_id(), controller);
    }
    connect(&m_ans, &PageAnswer::s_answer, this, &AsyncPageImageResponse::handleDone);
    controller->getAsyncPage(m_req, &m_ans);
    qWarning("Provider: requested: %i, (%i, %i), %s", m_req.index(), m_req.width(), m_req.height(), m_req.book_filename().toLocalFile().toStdString().c_str());
}
void AsyncPageImageResponse::handleDone(QImage img) {
    m_image = img;
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    qWarning("Provider: sending %i, (%i, %i)", m_req.index(), m_image.width(), m_image.height());
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
