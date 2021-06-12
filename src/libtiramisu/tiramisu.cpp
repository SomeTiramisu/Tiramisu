#include "tiramisu.h"

Tiramisu::Tiramisu(QUrl filename)
    : QObject(nullptr)
{
    qRegisterMetaType<cv::Rect>();

    m_filename = filename;
    m_preloader = new PagePreloader();
    m_scheduler = new PageScheduler(m_preloader);
}

Tiramisu::~Tiramisu() {
    m_scheduler->deleteLater();
    m_preloader->deleteLater();
}

QImage Tiramisu::get(PageRequest req) {
    if(req.filename() != m_filename) {
        setFilename(req.filename());
    }
    return m_scheduler->getAsyncPage(req);
}

void Tiramisu::setFilename(const QUrl &filename) {
    m_filename = filename;
    m_scheduler->deleteLater();
    m_preloader->deleteLater();
    m_preloader = new PagePreloader(filename);
    m_scheduler = new PageScheduler(m_preloader);
    m_bookSize = m_preloader->size();
}

