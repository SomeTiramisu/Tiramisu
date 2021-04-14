#ifndef IMAGERUNNABLE_H
#define IMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "book.h"
#include "helper.h"

class ImageRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ImageRunnable(Book &book, PageRequest req);
    ~ImageRunnable();
    void run() override;
private:
    Book &book;
    PageRequest req;

signals:
    void done(Page page);
};

#endif // IMAGERUNNABLE_H
