#include "imagerunnable.h"

#include "utils/imageproc.h"

ImageRunnable::ImageRunnable(Parser &book, PageRequest req)
    : m_book(book),
      m_req(req)
{
}

ImageRunnable::~ImageRunnable() {
}

void ImageRunnable::run() {
    cv::Mat img = m_book.at(m_req.index());
    if (!img.empty()) {
        ImageProc::classicProcess(img, img, m_req.width(), m_req.height());
    }
    qWarning("Runnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit done(m_req, ImageProc::toQImage(img));
}
