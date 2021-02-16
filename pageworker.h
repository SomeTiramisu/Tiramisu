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
    QPixmap* requestImage(QString book_filename, QString bg_filename, int index, int width, int height);
private:
    Book *book;

public slots:
    void addImage(QString book_filename, QString bg_filename, int index, int width, int height);

signals:
    void imageReady(QPixmap* img, int index);
};

#endif // PAGEWORKER_H
