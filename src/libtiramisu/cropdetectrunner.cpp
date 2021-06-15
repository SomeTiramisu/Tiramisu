#include "cropdetectrunner.h"

#include "utils/imageproc.h"

CropDetectRunner::~CropDetectRunner() {
    get();
}

void CropDetectRunner::run(Parser *parser, int index) {
    reset();
    m_valid = true;
    m_th = std::make_unique<std::thread>(&CropDetectRunner::cropDetect, parser, index, m_res.get());
}

PngPair CropDetectRunner::get() const {
    if(not m_valid) {
        throw "Runner not valid";
    }
    if(m_th) {
        if(m_th->joinable()) {
            m_th->join();
        }
        //m_th.reset();
    }
    return *m_res.get();
}

void CropDetectRunner::reset() {
    if(m_th) {
        if(m_th->joinable()) {
            m_th->join();
        }
        m_th.reset();
    }
    m_res = std::make_unique<PngPair>();
    m_valid = false;
}

bool CropDetectRunner::valid() const {
    return m_valid;
}

void CropDetectRunner::cropDetect(Parser *parser, int index, PngPair* res) {
    ByteVect png = parser->at(index);
    cv::Mat img = ImageProc::fromVect(png);
    cv::Rect roi;
    if (not img.empty() and index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    //qWarning("CropDetectRunnable: running: %i", index);
    *res = PngPair{png, roi};
}
