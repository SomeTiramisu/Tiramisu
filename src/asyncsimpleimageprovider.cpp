#include "asyncsimpleimageprovider.h"

#include "utils/utils.h"
#include "utils/imageproc.h"
#include <opencv2/imgproc.hpp>

AsyncSimpleImageResponseRunnable::AsyncSimpleImageResponseRunnable(const PageRequest& req, const QSize& requestedSize, Parser* parser)
    : m_parser(parser),
      m_req(req),
      m_requestedSize(requestedSize)
{}

void AsyncSimpleImageResponseRunnable::run() {
    m_parser->tryReset(m_req.book_filename(), true);
    cv::Mat mat = m_parser->at(m_req.index());
    if(not mat.empty()) {
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGBA);
    }
    QImage img = ImageProc::toQImage(mat);
    emit done(img);
}


AsyncSimpleImageResponse::AsyncSimpleImageResponse(const QString &id, const QSize &requestedSize, Parser* parser, QThreadPool& pool)
    : m_req(Utils::decodeId(id))
{
    AsyncSimpleImageResponseRunnable* runnable = new AsyncSimpleImageResponseRunnable(m_req, requestedSize, parser);
    connect(runnable, &AsyncSimpleImageResponseRunnable::done, this, &AsyncSimpleImageResponse::handleDone);
    pool.start(runnable);
    qWarning("SimpleProvider: requested: %i, %s", m_req.index(), m_req.book_filename().toLocalFile().toStdString().c_str());
}
void AsyncSimpleImageResponse::handleDone(QImage img) {
    m_image = img;
    emit finished();
}

QQuickTextureFactory *AsyncSimpleImageResponse::textureFactory() const {
    qWarning("SimpleProvider: sending %i, (%i, %i)", m_req.index(), m_image.width(), m_image.height());
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

AsyncSimpleImageProvider::AsyncSimpleImageProvider()
    : QQuickAsyncImageProvider()
{}

AsyncSimpleImageProvider::~AsyncSimpleImageProvider() {
}

QQuickImageResponse *AsyncSimpleImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    AsyncSimpleImageResponse *response = new AsyncSimpleImageResponse(id, requestedSize, &parser, pool);
    return response;
}
