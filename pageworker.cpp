#include "pageworker.h"
#include "image.h"

void ImageWorker::setBook(Book *b) {
    book = b;
}
void ImageWorker::addImage(double width, double height) {
    char* buf = book->getNext();
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(width, height);
    emit imageReady(img.toQPixmap());
}
