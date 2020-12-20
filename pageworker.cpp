#include "pageworker.h"
#include "image.h"
#define WIDTH 1200
#define HEIGHT 1920
#define IMAGE_PRELOAD 10

void ImageWorker::setBook(Book *b) {
    book = b;
}

void ImageWorker::addImage(int index) {
    qWarning("requesting %i", index);
    char* buf = book->getAt(index);
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(WIDTH, HEIGHT);
    emit imageReady(img.toQPixmap(), index);
}
