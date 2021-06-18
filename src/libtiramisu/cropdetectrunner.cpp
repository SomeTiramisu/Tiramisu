#include "cropdetectrunner.h"

#include "utils/imageproc.h"

CropDetectRunner::CropDetectRunner(Parser* parser)
    : m_parser(parser)
{}

void CropDetectRunner::run(int index) {
    if(m_index != index || m_index == 0) {
        m_index = index;
        m_future = std::async(&CropDetectRunner::cropDetect, m_parser, m_index);
    }
}

PngPair CropDetectRunner::get(int index) {
    run(index);
    if(m_future.valid()) {
        m_res = m_future.get();
    }
    return m_res;
}

void CropDetectRunner::clear() {
    m_index = 0;
    m_res = PngPair();
    m_future = std::future<PngPair>();
}

PngPair CropDetectRunner::cropDetect(Parser *parser, int index) {
    ByteVect png = parser->at(index);
    cv::Mat img = ImageProc::fromVect(png);
    cv::Rect roi;
    if (not img.empty() and index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    //qWarning("CropDetectRunnable: running: %i", index);
    return PngPair{png, roi};
}
