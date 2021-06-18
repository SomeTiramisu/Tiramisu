#include "tiramisu.h"

cv::Mat Tiramisu::get(const PageRequest& req) {
    if(req.filename != m_filename) {
        setFilename(req.filename);
    } else if(req.filename.empty()) {
        return cv::Mat();
    }
    return m_scheduler.getPage(req);
}

void Tiramisu::setFilename(const Path& filename) {
    m_filename = filename;
    m_preloader = PagePreloader(filename);
    m_scheduler = PageScheduler(&m_preloader);
    m_bookSize = m_preloader.size();
}

