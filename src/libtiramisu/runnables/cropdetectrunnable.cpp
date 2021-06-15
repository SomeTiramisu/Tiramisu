#include "cropdetectrunnable.h"

#include "../utils/imageproc.h"

CropDetectRunnable::CropDetectRunnable(Parser* parser, int index)
    : m_index(index),
      m_parser(parser)
{}

void CropDetectRunnable::run() {
    std::vector<char> png = m_parser->at(m_index);
    cv::Mat img = ImageProc::fromVect(png);
    cv::Rect roi;
    if (not img.empty() and m_index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    qWarning("CropDetectRunnable: running: %i", m_index);
    emit roiReady(m_index, png, roi);
}
