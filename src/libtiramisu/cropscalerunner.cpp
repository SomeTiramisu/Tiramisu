#include "cropscalerunner.h"

#include "utils/imageproc.h"

CropScaleRunner::CropScaleRunner(PagePreloader* preloader)
    : m_preloader(preloader)
{}

void CropScaleRunner::run(const PageRequest& req) {
    if(m_req != req) {
        m_req = req;
        m_future = std::async(std::launch::async, &CropScaleRunner::cropScale, m_preloader, m_req);
    }
}

PagePair CropScaleRunner::get(const PageRequest& req) {
    run(req);
    if(m_future.valid()) {
        m_res = m_future.get();
    }
    return m_res;
}

void CropScaleRunner::clear() {
    m_req = PageRequest();
    m_res = PagePair();
    m_future = std::future<PagePair>();
}

PagePair CropScaleRunner::cropScale(PagePreloader* preloader, const PageRequest& req) {
    PngPair p = preloader->at(req.index);
    cv::Mat img = ImageProc::fromVect(p.png);
    if (not img.empty()) {
        ImageProc::cropScaleProcess(img, img, p.roi, req.width, req.height);
    }
    //qWarning("CropScaleRunnable: running: %i", index);
    return PagePair{img, req};
}
