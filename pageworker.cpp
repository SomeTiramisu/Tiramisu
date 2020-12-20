#include "pageworker.h"
#include "image.h"

void ImageWorker::setBook(Book *b) {
    book = b;
}
void ImageWorker::setImageSize(int w, int h) {
    width = w;
    height = h;
}

void ImageWorker::addImage(int index) {
    qWarning("requesting %i", index);
    char* buf = book->getAt(index);
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(width, height);
    emit imageReady(img.toQPixmap(), index);
}
