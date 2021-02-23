#ifndef PAGEWORKER_H
#define PAGEWORKER_H

#include <QObject>
#include <book.h>
#include <memory>

class ImageWorker : public QObject
{
    Q_OBJECT
public:
    ImageWorker();
    ~ImageWorker();
    Page requestImage(QUrl book_filename, QUrl bg_filename, int index, int width, int height);
private:
    Book book;

public slots:
    void addImage(QUrl book_filename, QUrl bg_filename, int index, int width, int height);

signals:
    void imageReady(Page page);
};

#endif // PAGEWORKER_H
