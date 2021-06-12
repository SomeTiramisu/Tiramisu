#include "cropscalerunnable.h"

#include "../utils/imageproc.h"

CropScaleRunnable::CropScaleRunnable(PagePreloader* preloader, const PageRequest& req)
    : m_preloader(preloader),
      m_req(req)
{}

void CropScaleRunnable::run() {
    PngPair pair = m_preloader->at(m_req.index());
    cv::Mat img = ImageProc::fromByteArray(pair.png);
    if (not img.empty()) {
        ImageProc::cropScaleProcess(img, img, pair.roi, m_req.width(), m_req.height());
    }
    qWarning("CropScaleRunnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit imageReady(m_req, ImageProc::toQImage(img));
}
