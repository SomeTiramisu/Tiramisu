#include "pageworker.h"
#include "image.h"

void ImageWorker::addImage(Book* book, QString bg_filename, int index, int width, int height) {
    qWarning("requesting %i", index);
    char* buf = book->getAt(index);
    long long length = book->getLength(index);
    ImageProc img = ImageProc(buf, length, bg_filename.toStdString());
    delete[] buf;
    try {
        img.process(width, height);
        emit imageReady(img.toQPixmap(), index);
    }  catch (...) {
        qWarning("Something goes wrong with %i", index);
        QPixmap* r = new QPixmap(width, height);
        emit imageReady(r, index);
    }
}
