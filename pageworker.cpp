#include "pageworker.h"
#include "image.h"
#define WIDTH 1200
#define HEIGHT 1920
#define IMAGE_PRELOAD 10

void ImageWorker::setBook(Book *b) {
    book = b;
}

void ImageWorker::addNextImage() {
    char* buf = book->getNext();
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(WIDTH, HEIGHT);
    emit nextImageReady(img.toQPixmap());
}

void ImageWorker::addPreviousImage() {
    char* buf = book->getPrevious();
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(WIDTH, HEIGHT);
    emit previousImageReady(img.toQPixmap());
}
