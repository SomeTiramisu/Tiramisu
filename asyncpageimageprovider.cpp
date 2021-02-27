#include <asyncpageimageprovider.h>
#include <qquickimageprovider.h>
#include <QImage>
#include <QThreadPool>
#include "pageworker.h"
#include "imageproc.h"
#include "backend.h"


AsyncPageImageResponseRunnable::AsyncPageImageResponseRunnable(const QString &id, const QSize &requestedSize, QUrl book_filename, int index, int width, int height)
    : m_id(id), m_requestedSize(requestedSize), m_book_filename(book_filename), m_index(index), m_width(width), m_height(height) {
}

void AsyncPageImageResponseRunnable::run() {
    ImageWorker w;
    Page p = w.requestImage(m_book_filename, m_index, m_width, m_height);
    QImage image = ImageProc::toQImage(p.img);
    if (m_requestedSize.isValid())
        image = image.scaled(m_requestedSize);
    emit done(image);
}


AsyncPageImageResponse::AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QThreadPool *pool, Backend *backend) {
    AsyncPageImageResponseRunnable *runnable =
            new AsyncPageImageResponseRunnable(id, requestedSize, backend->bookFilename(), backend->pageIndex(), backend->width(), backend->height());
    connect(runnable, &AsyncPageImageResponseRunnable::done, this, &AsyncPageImageResponse::handleDone);
    pool->start(runnable);
}
void AsyncPageImageResponse::handleDone(QImage image) {
    m_image = image;
    emit finished();
}

QQuickTextureFactory *AsyncPageImageResponse::textureFactory() const {
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncPageImageProvider::AsyncPageImageProvider(Backend *b)
    : QQuickAsyncImageProvider() {
    backend = b;
}

QQuickImageResponse *AsyncPageImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncPageImageResponse *response = new AsyncPageImageResponse(id, requestedSize, &pool, backend);
    return response;
}
