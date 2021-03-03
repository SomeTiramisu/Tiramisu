#include "pageworker.h"
#include "imageproc.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

ImageWorker::ImageWorker()
    : book(QUrl())
{
}

ImageWorker::~ImageWorker() {
}

void ImageWorker::addImage(QUrl book_filename, int index, int width, int height) {
    emit imageReady(requestImage(book_filename, index, width, height));
}

Page ImageWorker::requestImage(QUrl book_filename, int index, int width, int height) {
    qWarning("requesting %i, %i, %i", index, width, height);
    if (book_filename != book.getFilename()) {
        book = Book(book_filename);
    }
    Page p = book.getAt(index, width, height);
    qWarning("page recived lib: %i, w: %i, h: %i", 0, p.img.rows , p.img.cols);
    if (!p.img.empty()) {
        ImageProc::classicProcess(p.img, p.img, width, height);
        //cv::cvtColor(p.img, p.img, cv::COLOR_BGR2RGBA);
    }
    return p;
}
