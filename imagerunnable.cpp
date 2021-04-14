#include "imagerunnable.h"

#include "imageproc.h"

ImageRunnable::ImageRunnable(Book &book, PageRequest req)
    : book(book),
      req(req)
{
}

ImageRunnable::~ImageRunnable() {
}

void ImageRunnable::run() {
    Page p = book.getAt(req.index, req.width, req.height);
    if (!p.img.empty()) {
        ImageProc::classicProcess(p.img, p.img, req.width, req.height);
    }
    emit done(p);
}
