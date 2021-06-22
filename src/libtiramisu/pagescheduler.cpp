#include "pagescheduler.h"

#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler(PagePreloader* preloader)
    : m_preloader(preloader),
      m_imagePreload(0)
{
    m_pages.resize(m_preloader->size());
    for (int i=0; i<m_preloader->size(); i++) {
        m_pages.at(i) = CropScaleRunner(m_preloader);
    };
    qWarning("scheduler created");
}

void PageScheduler::at(const PageRequest& req, const Slot<PagePair> slot) {
    seekPages(req);
    int index = req.index;
    int bookSize = m_preloader->size();
    if (index<0 || index >= bookSize) {
        slot(PagePair());
        return;
    }
    m_pages.at(req.index).get(req, slot);
}

void PageScheduler::seekPages(const PageRequest& req) {
    for(int i=0; i<(int)m_pages.size(); i++) {
        if((req.index - m_imagePreload <= i) && (i <= req.index + m_imagePreload)) {
            PageRequest nreq{i, req.width, req.height, req.filename};
            //qWarning("SEEK %i", i);
            m_pages.at(i).get(nreq);
        } else {
            m_pages.at(i).clear();
        }
    }
}
