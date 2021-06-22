#include "cropdetectrunner.h"

#include "utils/imageproc.h"

CropDetectRunner::CropDetectRunner(Parser* parser)
    : m_parser(parser)
{
    m_slot = [](const PngPair& res){(void)res;};
}

void CropDetectRunner::run() {
    m_thread = std::thread([this]{this->handleCropDetect(cropDetect(this->m_parser->at(this->m_index), this->m_index));});
    m_thread.detach();
}

void CropDetectRunner::get(int index, const Slot<PngPair>& slot) {
    if(m_index<0) { //not requested
        m_index = index;
        run();
        return;
    }
    if(m_index == index && m_res != PngPair()) { //requested, not recieved
        m_slot=slot;
        return;
    }
    m_slot(m_res);
}

void CropDetectRunner::get(int index) {
    m_slot = Slot<PngPair>();
    if(m_index<0) { //not requested
        m_index = index;
        run();
        return;
    }
}

void CropDetectRunner::clear() {
    m_index = -1;
    m_res = PngPair();
}

PngPair CropDetectRunner::cropDetect(const ByteVect& png, int index) {
    cv::Mat img(ImageProc::fromVect(png));
    cv::Rect roi;
    if (not img.empty() and index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    qWarning("CropDetectRunnable: running: %i", index);
    return PngPair{png, roi};
}

void CropDetectRunner::handleCropDetect(const PngPair &res) {
    m_res = res;
    m_slot(m_res);
}
