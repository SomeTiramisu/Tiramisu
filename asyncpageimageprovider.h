#ifndef ASYNCPAGEIMAGEPROVIDER_H
#define ASYNCPAGEIMAGEPROVIDER_H

#include <qquickimageprovider.h>
#include <QImage>
#include <QThreadPool>
#include "backend.h"

class AsyncPageImageResponseRunnable : public QObject, public QRunnable
{
    Q_OBJECT
signals:
    void done(QImage image);

public:
    AsyncPageImageResponseRunnable(const QString &id, const QSize &requestedSize, const QUrl book_filename, const int index, const int width, const int height);
    void run() override;

private:
    QString m_id;
    QSize m_requestedSize;
    QUrl m_book_filename;
    int m_index;
    int m_width;
    int m_height;
};

class AsyncPageImageResponse : public QQuickImageResponse
{
    public:
        AsyncPageImageResponse(const QString &id, const QSize &requestedSize, QThreadPool *pool, Backend *backend);
        void handleDone(QImage image);
        QQuickTextureFactory *textureFactory() const override;
        QImage m_image;
};

class AsyncPageImageProvider : public QQuickAsyncImageProvider
{
public:
    AsyncPageImageProvider(Backend *b);
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    QThreadPool pool;
    Backend *backend;
};




#endif // ASYNCPAGEIMAGEPROVIDER_H
