#include "pageworker.h"
#include "imageproc.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

ImageWorker::ImageWorker()
    : book(QUrl())
{
}

ImageWorker::~ImageWorker() {
}

void ImageWorker::addImage(QUrl book_filename, QUrl bg_filename, int index, int width, int height) {
    emit imageReady(requestImage(book_filename, bg_filename, index, width, height));
}

Page ImageWorker::requestImage(QUrl book_filename, QUrl bg_filename, int index, int width, int height) {
    qWarning("requesting %i, %i, %i", index, width, height);
    if (book_filename != book.getFilename()) {
        book = Book(book_filename);
    }
    Page p = book.getAt(index, width, height);
    cv::Mat bg = cv::imread(bg_filename.toLocalFile().toStdString(), cv::IMREAD_COLOR);
    if (!p.img.empty())
        ImageProc::classicProcess(p.img, bg, p.img, width, height);
    return p;
}
