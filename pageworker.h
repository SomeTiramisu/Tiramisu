#ifndef PAGEWORKER_H
#define PAGEWORKER_H

#include <QObject>
#include <book.h>

class ImageWorker : public QObject
{
    Q_OBJECT
public:
    void setBook(Book* b);
    void setImageSize(int w, int h);
private:
    Book* book;
    int height;
    int width;

public slots:
    void addImage(int index);

signals:
    void imageReady(QPixmap *img, int index);
};

#endif // PAGEWORKER_H
