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
    void handleDone(QImage image);
    QImage m_image;
};

class AsyncPageImageProvider : public QQuickAsyncImageProvider
{
public:
    AsyncPageImageProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    PageController *controller;
};

#endif // ASYNCPAGEIMAGEPROVIDER_H
