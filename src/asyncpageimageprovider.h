#ifndef ASYNCPAGEIMAGEPROVIDER_H
#define ASYNCPAGEIMAGEPROVIDER_H

#include "qquickimageprovider.h"
#include <QImage>
#include "pagecontroller.h"

class AsyncPageImageResponse : public QQuickImageResponse
{
public:
    AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QHash<QString, PageController*>& controllers);
    QQuickTextureFactory *textureFactory() const override;
    QImage m_image;
public slots:
    void handleDone(QImage img);

};

class AsyncPageImageProvider : public QQuickAsyncImageProvider
{
public:
    AsyncPageImageProvider();
    ~AsyncPageImageProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    QHash<QString, PageController*> controllers;
};

#endif // ASYNCPAGEIMAGEPROVIDER_H
