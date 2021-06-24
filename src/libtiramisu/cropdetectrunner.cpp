#include "cropdetectrunner.h"

#include "utils/imageproc.h"
#include "threadpool.h"

CropDetectRunner::CropDetectRunner(Parser* parser, QThreadPool* pool)
    : m_parser(parser),
      m_pool(pool)
{
    m_slot = [](const PngPair& res){(void)res;};
}

void CropDetectRunner::run() {
    //std::thread thread([this]{this->handleCropDetect(cropDetect(this->m_parser->at(this->m_index), this->m_index));});
    //thread.detach();
    m_pool->start(QRunnable::create([this]{
        this->m_res = cropDetect(this->m_parser->at(this->m_index), this->m_index);
        //qWarning("DEBUG");
        this->m_slot(this->m_res);
    }));
}

void CropDetectRunner::get(int index, const Slot<PngPair>& slot) {
    if(m_index != index || m_index<0) { //not requested
        m_index = index;
        m_slot = slot;
        run();
        return;
    }
    if(m_index == index && m_res == PngPair()) { //requested, not recieved
        m_slot=slot;
        return;
    }
    slot(m_res); //recieved
}

void CropDetectRunner::get(int index) {
    //m_slot = [](const PngPair& res){(void)res;};
    if(m_index != index || m_index<0) { //not requested
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
