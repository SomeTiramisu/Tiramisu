#include "imagerunnable.h"

#include "utils/imageproc.h"

ImageRunnable::ImageRunnable(Parser &book, PageRequest req)
    : book(book),
      req(req)
{
}

ImageRunnable::~ImageRunnable() {
}

void ImageRunnable::run() {
    cv::Mat img = book.getAt(req.index);
    if (!img.empty()) {
        ImageProc::classicProcess(img, img, req.width, req.height);
    }
    qWarning("Runnable: running: %i", req.index);
    emit done(req, ImageProc::toQImage(img));
}
