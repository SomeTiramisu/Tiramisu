#include "pageworker.h"
#include "imageproc.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

ImageWorker::ImageWorker()
    : book("dummy")
{
}

ImageWorker::~ImageWorker() {
}

void ImageWorker::addImage(QString book_filename, QString bg_filename, int index, int width, int height) {
    emit imageReady(requestImage(book_filename, bg_filename, index, width, height));
}

Page ImageWorker::requestImage(QString book_filename, QString bg_filename, int index, int width, int height) {
    qWarning("requesting %i, %i, %i", index, width, height);
    if (book_filename.toStdString() != book.getFilename()) {
        book = Book(book_filename.toStdString());
    }
    Page p = book.getAt(index, width, height);
    cv::Mat bg = cv::imread(bg_filename.toStdString(), cv::IMREAD_COLOR);
    //try {
        ImageProc::classicProcess(p.img, bg, p.img, width, height);
        return p;
    //}  catch (...) {
    //    qWarning("Something goes wrong with %i", index);
    //    QPixmap* r = new QPixmap(width, height);
    //    return r;
    //}
}
