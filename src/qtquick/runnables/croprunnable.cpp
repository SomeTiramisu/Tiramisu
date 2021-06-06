#include "croprunnable.h"

#include "../utils/imageproc.h"

CropRunnable::CropRunnable(Parser* parser, int index)
    : m_index(index),
      m_parser(parser)
{}

void CropRunnable::run() {
    cv::Mat img = ImageProc::fromByteArray(m_parser->at(m_index));
    if (not img.empty()) {
        ImageProc::cropProcess(img, img);
    }
    qWarning("CropRunnable: running: %i", m_index);
    emit pngReady(m_index, ImageProc::toPng(img));
}
