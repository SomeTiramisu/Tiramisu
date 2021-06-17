#include "tiramisu.h"

Tiramisu::Tiramisu() {
    m_preloader = std::make_unique<PagePreloader>();
    m_scheduler = std::make_unique<PageScheduler>(m_preloader.get());
}

cv::Mat Tiramisu::get(PageRequest req) {
    if(req.filename() != m_filename) {
        setFilename(req.filename());
    }
    return m_scheduler->getPage(req);
}

void Tiramisu::setFilename(const Path& filename) {
    m_filename = filename;
    m_preloader = std::make_unique<PagePreloader>(filename);
    m_scheduler = std::make_unique<PageScheduler>(m_preloader.get());
    m_bookSize = m_preloader->size();
}
