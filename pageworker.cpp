#include "pageworker.h"
#include "image.h"

ImageWorker::ImageWorker() {
    book = nullptr;
}

ImageWorker::~ImageWorker() {
    delete book;
}

void ImageWorker::addImage(QString book_filename, QString bg_filename, int index, int width, int height) {
    emit imageReady(requestImage(book_filename, bg_filename, index, width, height), index);
}

QPixmap* ImageWorker::requestImage(QString book_filename, QString bg_filename, int index, int width, int height) {
    qWarning("requesting %i", index);
    if (book == nullptr || book_filename.toStdString() != book->getFilename()) {
        delete book;
        book = new Book(book_filename.toStdString());
    }
    char* buf = book->getAt(index);
    long long length = book->getLength(index);
    ImageProc img = ImageProc(buf, length, bg_filename.toStdString());
    delete[] buf;
    try {
        img.process(width, height);
        return img.toQPixmap();
    }  catch (...) {
        qWarning("Something goes wrong with %i", index);
        QPixmap* r = new QPixmap(width, height);
        return r;
    }
}
