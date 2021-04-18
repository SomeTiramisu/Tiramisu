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
    PageResponseCV p = book.getAt(req.index);
    if (!p.img.empty()) {
        ImageProc::classicProcess(p.img, p.img, req.width, req.height);
        p.width = req.width;
        p.height = req.height;
    }
    emit done(p);
}
