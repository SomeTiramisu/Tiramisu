#include "pageworker.h"
#include "image.h"

void ImageWorker::setBook(Book *b) {
    book = b;
}


void ImageWorker::addImage(int index, int width, int height) {
    qWarning("requesting %i", index);
    char* buf = book->getAt(index);
    unsigned int length = book->getLength();
    ImageProc img = ImageProc(buf, length);
    delete[] buf;
    try {
        img.process(width, height);
        emit imageReady(img.toQPixmap(), index);
    }  catch (...) {
        qWarning("Something goes wrong with %i", index);
        QPixmap* r = new QPixmap;
        emit imageReady(r, index);
    }
}
