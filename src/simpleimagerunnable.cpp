#include "simpleimagerunnable.h"

#include "utils/imageproc.h"
#include <opencv2/imgproc.hpp>

SimpleImageRunnable::SimpleImageRunnable(Parser &book, PageRequest req)
    : m_book(book),
      m_req(req)
{}

void SimpleImageRunnable::run() {
    cv::Mat img = m_book.at(m_req.index());
    if (not img.empty()) {
        cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);
        ImageProc::scale3(img, img, m_req.width(), m_req.height());
    }
    qWarning("SimpleRunnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit done(m_req, ImageProc::toQImage(img));
}
