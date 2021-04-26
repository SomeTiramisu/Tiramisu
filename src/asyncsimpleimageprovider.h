#ifndef ASYNCSIMPLEIMAGEPROVIDER_H
#define ASYNCSIMPLEIMAGEPROVIDER_H

#include "qquickimageprovider.h"
#include <QImage>
#include "pagecontroller.h"

class AsyncSimpleImageResponseRunnable: public QObject, public QRunnable
{
    Q_OBJECT
public:
    AsyncSimpleImageResponseRunnable(const PageRequest& req, const QSize& requestedSize, Parser* parser);
    ~AsyncSimpleImageResponseRunnable() {};
    void run() override;
private:
    Parser* m_parser;
    PageRequest m_req;
    QSize m_requestedSize;
signals:
    void done(QImage image);
};

class AsyncSimpleImageResponse : public QQuickImageResponse
{
public:
    AsyncSimpleImageResponse(const QString &id, const QSize &requestedSize, Parser* parser, QThreadPool& pool);
    QQuickTextureFactory *textureFactory() const override;
    void handleDone(QImage img);
private:
    const PageRequest m_req;
    QImage m_image;



};

class AsyncSimpleImageProvider : public QQuickAsyncImageProvider
{
public:
    AsyncSimpleImageProvider();
    ~AsyncSimpleImageProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    QThreadPool pool;
    Parser parser;
};

#endif // ASYNCPAGEIMAGEPROVIDER_H
