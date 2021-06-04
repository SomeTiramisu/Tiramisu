#include "scalerunnable.h"

#include "utils/imageproc.h"

ScaleRunnable::ScaleRunnable(PagePreloader* preloader, const PageRequest& req)
    : m_preloader(preloader),
      m_req(req)
{}

void ScaleRunnable::run() {
    cv::Mat img = ImageProc::fromByteArray(m_preloader->at(m_req.index()));
    if (not img.empty()) {
        ImageProc::scaleProcess(img, img, m_req.width(), m_req.height());
    }
    qWarning("ScaleRunnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit imageReady(m_req, ImageProc::toQImage(img));
}
