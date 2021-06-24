#include "tiramisu.h"

void Tiramisu::get(const PageRequest& req) {
    if(req.filename != m_filename) {
        setFilename(req.filename);
    } else if(req.filename.empty()) {
        m_schedulerImageSlot(cv::Mat());
        return;
    }
    m_req = req;
    m_scheduler.at(req, [this](const PagePair& res){
        //qWarning("DEBUG3");
        if(this->m_req==res.req) {
            //qWarning("PASSED");
            this->m_schedulerImageSlot(res.img);
        }
    });
}

void Tiramisu::setFilename(const Path& filename) {
    m_filename = filename;
    m_preloader = PagePreloader(filename, &m_pool);
    m_scheduler = PageScheduler(&m_preloader, &m_pool);
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
