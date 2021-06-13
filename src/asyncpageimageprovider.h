#ifndef ASYNCPAGEIMAGEPROVIDER_H
#define ASYNCPAGEIMAGEPROVIDER_H

#include "qquickimageprovider.h"
#include <QImage>
#include "pagescheduler.h"

class AsyncPageImageResponse : public QQuickImageResponse
{
public:
    AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QHash<QString, PageScheduler*>& schedulers);
    QQuickTextureFactory *textureFactory() const override;
private:
    const PageRequest m_req;
    QSize m_requestedSize;
    PageAnswer m_ans;
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
    QHash<QString, PageScheduler*> schedulers;
};

#endif // ASYNCPAGEIMAGEPROVIDER_H
