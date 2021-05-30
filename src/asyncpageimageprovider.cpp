#include "asyncpageimageprovider.h"

#include "utils/utils.h"

AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QHash<QString, PageScheduler*>& schedulers) //reference sur pointeur
    : m_req(PageRequest::fromId(id)),
      m_requestedSize(requestedSize)
{
    Q_UNUSED(requestedSize)
    PageScheduler* scheduler = schedulers.value(m_req.controller_id());
    if (scheduler == nullptr) {
        scheduler = new PageScheduler(m_req.book_filename(), true, m_req.controller_preload());
        schedulers.insert(m_req.controller_id(), scheduler);
    } else if (scheduler->getBookFilename() != m_req.book_filename()) {
        scheduler->deleteLater();
        scheduler = new PageScheduler(m_req.book_filename(), true, m_req.controller_preload());
        schedulers.remove(m_req.controller_id());
        schedulers.insert(m_req.controller_id(), scheduler);
    }
    connect(&m_ans, &PageAnswer::s_answer, this, &AsyncPageImageResponse::handleDone);
    scheduler->getAsyncPage(m_req, &m_ans);
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
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, schedulers);
    return response;
}
