#include "cropscalerunner.h"

#include "utils/imageproc.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

CropScaleRunner::CropScaleRunner(PagePreloader* preloader)
    : m_preloader(preloader)
{
    m_slot = [](const PagePair& res){(void)res;};
}

void CropScaleRunner::run() {
    m_thread = std::thread([this]{this->m_preloader->at(this->m_req.index, [this](const PngPair& res){this->handleCropScale(cropScale(res, m_req));});});
    m_thread.detach();
}

void CropScaleRunner::get(const PageRequest& req, const Slot<PagePair>& slot) {
    if(m_req == PageRequest()) { //not requested
        m_req = req;
        m_slot = slot;
        run();
        return;
    }
    if(m_req == req && m_res == PagePair()) { //requested, not recieved
        m_slot=slot;
        return;
    }
    slot(m_res); //recieved
}

void CropScaleRunner::get(const PageRequest &req) {
    m_slot = [](const PagePair& res){(void)res;};
    if(m_req == PageRequest()) { //not requested
        m_req = req;
        run();
        return;
    }
}

void CropScaleRunner::clear() {
    m_req = PageRequest();
    m_res = PagePair();
}

PagePair CropScaleRunner::cropScale(const PngPair& p, const PageRequest& req) {
    cv::Mat img(ImageProc::fromVect(p.png));
    if (not img.empty()) {
        ImageProc::cropScaleProcess(img, img, p.roi, req.width, req.height);
    }
    qWarning("CropScaleRunnable: running: %i", req.index);
    return PagePair{img, req};
}

void CropScaleRunner::handleCropScale(const PagePair &res) {
    m_res = res;
    qWarning("DEBUG2");
    m_slot(m_res);
    //cv::Mat img = cv::imread("/home/guillaume/reader/000.jpg", IMREAD_COLOR);
    //cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);
    //m_slot(PagePair{img, res.req});
}
