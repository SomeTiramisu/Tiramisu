#include "tiramisu.h"

void Tiramisu::get(const PageRequest& req, Slot<cv::Mat> slot) {
    if(req.filename != m_filename) {
        setFilename(req.filename);
    } else if(req.filename.empty()) {
        slot(cv::Mat());
        return;
    }
    m_req = req;
    m_slot = slot;
    m_scheduler.at(req, [this](const PagePair& res){this->handleSchedulerAt(res);});
}

void Tiramisu::setFilename(const Path& filename) {
    m_filename = filename;
    m_preloader = PagePreloader(filename);
    m_scheduler = PageScheduler(&m_preloader);
    m_bookSize = m_preloader.size();
}

void Tiramisu::handleSchedulerAt(const PagePair& res) {
    qWarning("DEBUG3");
    if(m_req==res.req) {
        if(res.img.empty()) {
            qWarning("DEBUG3 EMPTY");
        }
        m_slot(res.img);
    }
}
