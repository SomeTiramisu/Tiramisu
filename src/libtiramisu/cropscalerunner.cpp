#include "cropscalerunner.h"

#include "utils/imageproc.h"

CropScaleRunner::CropScaleRunner(Parser* parser, QThreadPool* pool)
    : m_pool(pool),
      m_parser(parser)
{
    m_slot = [](const PagePair& res){(void)res;};
}

void CropScaleRunner::runScale() {
    m_pool->start(QRunnable::create([this]{
        if(!m_pngRes.has_value()) {
            m_pngRes = cropDetect(m_parser->at(m_req.index), m_req.index);
        }
        m_pageRes = cropScale(m_pngRes.value(), m_req);
        //qWarning("DEBUG2");
        this->m_slot(this->m_pageRes.value());
    }));
}

void CropScaleRunner::runDetect() {
    m_pool->start(QRunnable::create([this]{
        m_pngRes = cropDetect(m_parser->at(m_req.index), m_req.index);
    }));
}

void CropScaleRunner::get(const PageRequest& req) {
    if(m_req!=req || !m_pageRes.has_value()) { //not requested
        m_req = req;
        runScale();
        return;
    }
    if(m_req==req) {  //requested, not recieved
        //return;
    }
    m_slot(m_pageRes.value()); //recieved
}

void CropScaleRunner::preload(const PageRequest &req) {
    if(!m_pngRes.has_value()) { //not requested
        m_req = req;
        runDetect();
        return;
    }
}

void CropScaleRunner::clear() {
    m_pageRes.reset();
}

void CropScaleRunner::connectSlot(const Slot<PagePair> &slot) {
    m_slot = slot;
}

PagePair CropScaleRunner::cropScale(const PngPair& p, const PageRequest& req) {
    cv::Mat img(ImageProc::fromVect(p.png));
    if (not img.empty()) {
        ImageProc::cropScaleProcess(img, img, p.roi, req.width, req.height);
    }
    qWarning("CropScaleRunnable: running: %i", req.index);
    return PagePair{img, req};
}

PngPair CropScaleRunner::cropDetect(const ByteVect &png, int index) {
    cv::Mat img(ImageProc::fromVect(png));
    cv::Rect roi;
    if (not img.empty() and index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    qWarning("CropDetectRunnable: running: %i", index);
    return PngPair{png, roi};
}
