#ifndef ASYNCPAGEIMAGEPROVIDER_H
#define ASYNCPAGEIMAGEPROVIDER_H

#include <qquickimageprovider.h>
#include <QImage>
#include "pagecontroller.h"

class AsyncPageImageResponse : public QQuickImageResponse
{
public:
    AsyncPageImageResponse(const QString &id, const QSize &requestedSize, PageController *&controller);
    QQuickTextureFactory *textureFactory() const override;
    QImage m_image;
public slots:
    void handleDone(PageResponseQ image);

};

class AsyncPageImageProvider : public QQuickAsyncImageProvider
{
public:
    AsyncPageImageProvider();
    ~AsyncPageImageProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    PageController *controller;
};

#endif // ASYNCPAGEIMAGEPROVIDER_H
