#include "classicimagerunnable.h"

#include "../utils/imageproc.h"

ClassicImageRunnable::ClassicImageRunnable(PagePreloader* preloader, const PageRequest& req)
    : m_preloader(preloader),
      m_req(req)
{}

void ClassicImageRunnable::run() {
    cv::Mat img = ImageProc::fromByteArray(m_preloader->at(m_req.index()));
    if (not img.empty()) {
        ImageProc::classicProcess(img, img, m_req.width(), m_req.height());
    }
    qWarning("ClassicRunnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit imageReady(m_req, ImageProc::toQImage(img));
}
