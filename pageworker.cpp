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
    emit imageReady(requestImage(book_filename, bg_filename, index, width, height), index);
}

QPixmap* ImageWorker::requestImage(QString book_filename, QString bg_filename, int index, int width, int height) {
    qWarning("requesting %i, %i, %i", index, width, height);
    if (book_filename.toStdString() != book.getFilename()) {
        book = Book(book_filename.toStdString());
    }
    cv::Mat img = book.getAt(index);
    cv::Mat bg = cv::imread(bg_filename.toStdString(), cv::IMREAD_COLOR);
    //try {
        ImageProc::classicProcess(img, bg, img, width, height);
        return ImageProc::toQPixmap(img);
    //}  catch (...) {
    //    qWarning("Something goes wrong with %i", index);
    //    QPixmap* r = new QPixmap(width, height);
    //    return r;
    //}
}
