#include "tiramisu.h"

Tiramisu::Tiramisu() {
    m_scheduler.connectPageSlot([this](const PagePair& res){
        //qWarning("DEBUG3");
        if(m_req==res.req) {
            //qWarning("PASSED");
            m_schedulerImageSlot(res.img);
        }
    });
    m_scheduler.connectSizeSlot([this](const int& res){
        m_bookSizeSlot(res);
    });
}

void Tiramisu::get(const PageRequest& req) {
    if(req.filename.empty()) {
        m_schedulerImageSlot(cv::Mat());
        return;
    }
    m_req = req;
    m_scheduler.at(req);
}

void Tiramisu::connectPreloaderProgress(const Slot<int>& slot) {
    m_preloaderProgressSlot = slot;
}
void Tiramisu::connectBookSize(const Slot<int>& slot) {
    m_bookSizeSlot = slot;
}
void Tiramisu::connectImage(const Slot<cv::Mat>& slot) {
    m_schedulerImageSlot = slot;
}
