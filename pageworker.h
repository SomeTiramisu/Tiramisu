#ifndef PAGEWORKER_H
#define PAGEWORKER_H

#include <QObject>
#include <book.h>
#include <memory>

class ImageWorker : public QObject
{
    Q_OBJECT
public:
    void setBook(Book* b);
private:
    Book* book;

public slots:
    void addImage(int index, int width, int height);

signals:
    void imageReady(QPixmap* img, int index);
};

#endif // PAGEWORKER_H
