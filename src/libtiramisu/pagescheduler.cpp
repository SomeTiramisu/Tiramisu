#include "pagescheduler.h"

#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler(PagePreloader* preloader)
    : m_preloader(preloader),
      m_imagePreload(20)
{
    m_pages.resize(m_preloader->size());
    for (int i=0; i<m_preloader->size(); i++) {
        m_pages.at(i) = CropScaleRunner(m_preloader);
    };
    qWarning("scheduler created");
}

cv::Mat PageScheduler::getPage(const PageRequest& req) {
    seekPages(req);
    int index = req.index;
    int book_size = m_preloader->size();
    if (index<0 || index >= book_size) {
        return cv::Mat();
    }
    return m_pages.at(index).get(req).img;
}

void PageScheduler::seekPages(const PageRequest& req) {
    for(int i=0; i<(int)m_pages.size(); i++) {
        if((req.index - m_imagePreload <= i) && (i <= req.index + m_imagePreload)) {
            PageRequest nreq{i, req.width, req.height, req.filename};
            //qWarning("SEEK %i", i);
            m_pages.at(i).run(nreq);
        } else {
            m_pages.at(i).clear();
        }
    }
}
