#ifndef PAGEIMAGEPROVIDER_H
#define PAGEIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QThread>
//#include "book.h"
//#include "backend.h"
#include "pagecontroller.h"

class PageImageProvider :  public QQuickImageProvider
{

public:
    PageImageProvider(Backend *b);
    ~PageImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    PageController controller;
};


#endif // PAGEIMAGEPROVIDER_H
