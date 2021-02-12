#ifndef PAGEIMAGEPROVIDER_H
#define PAGEIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QThread>
#include "book.h"
#include "backend.h"
#include "pagecontroller.h"

class PageImageProvider :  public QQuickImageProvider
{

public:
    PageImageProvider();
    ~PageImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    Backend* getBackend();

private:
    Backend *backend;
    PageController *controller;
    int previousIndex;
};


#endif // PAGEIMAGEPROVIDER_H
