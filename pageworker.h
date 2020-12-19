#ifndef PAGEWORKER_H
#define PAGEWORKER_H

#include <QObject>
#include <book.h>

class ImageWorker : public QObject
{
    Q_OBJECT
public:
    void setBook(Book* b);
private:
    Book* book;

public slots:
    void addNextImage();
    void addPreviousImage();
    void addImage(int n);

signals:
    void nextImageReady(QPixmap *img);
    void previousImageReady(QPixmap *img);
};

#endif // PAGEWORKER_H
