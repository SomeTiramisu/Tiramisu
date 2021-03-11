#include "pageworker.h"
#include "imageproc.h"
//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>

ImageWorker::ImageWorker(QUrl book_filename)
    : book(book_filename)
{
}

ImageWorker::~ImageWorker() {
}

void ImageWorker::addImage(int index, int width, int height) {
    emit imageReady(requestImage(index, width, height));
}

Page ImageWorker::requestImage(int index, int width, int height) {
    qWarning("requesting %i, %i, %i", index, width, height);
    Page p = book.getAt(index, width, height);
    qWarning("page recived lib: %i, w: %i, h: %i", 0, p.img.rows , p.img.cols);
    if (!p.img.empty()) {
        ImageProc::classicProcess(p.img, p.img, width, height);
        //cv::cvtColor(p.img, p.img, cv::COLOR_BGR2RGBA);
    }
    return p;
}
