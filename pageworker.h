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
    void addImage(double width, double height);

signals:
    void imageReady(QPixmap *img);
};

#endif // PAGEWORKER_H
