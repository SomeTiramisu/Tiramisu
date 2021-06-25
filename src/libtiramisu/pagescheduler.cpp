#include "pagescheduler.h"

#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler()
    : m_imagePreload(5)
{
    m_pageSlot = [](const PagePair& res){(void)res;};
    m_sizeSlot = [](const int& res){(void)res;};
    qWarning("scheduler created");
}

PageScheduler::~PageScheduler() {
    m_pool.clear();
    m_pool.waitForDone();
}

void PageScheduler::at(const PageRequest& req) {
    if(req.filename!=m_filename) {
        m_pool.clear();
        m_pool.waitForDone();
        m_filename=req.filename;
        m_parser = Parser(m_filename);
        m_sizeSlot(m_parser.size());
        m_pages.resize(m_parser.size());
        for (int i=0; i<m_parser.size(); i++) {
            m_pages.at(i) = CropScaleRunner(&m_parser, &m_pool);
            m_pages.at(i).connectSlot(m_pageSlot);
        };
    }
    int index = req.index;
    int bookSize = m_parser.size();
    if (index<0 || index >= bookSize) {
        m_pageSlot(PagePair());
        return;
    }
    qWarning("scheduler.get(%i)", req.index);
    m_pages.at(req.index).get(req);
    seekPages(req);
}

void PageScheduler::seekPages(const PageRequest& req) {
    for(int i=0; i<(int)m_pages.size(); i++) {
        PageRequest nreq{i, req.width, req.height, req.filename};
        if((req.index - m_imagePreload <= i) && (i <= req.index + m_imagePreload)) {
            //PageRequest nreq{i, req.width, req.height, req.filename};
            //qWarning("SEEK %i", i);
            m_pages.at(i).get(nreq);
        } else {
            m_pages.at(i).preload(nreq);
            m_pages.at(i).clear();
        }
    }
}

void PageScheduler::connectPageSlot(const Slot<PagePair>& slot) {
    m_pageSlot = slot;
}

void PageScheduler::connectSizeSlot(const Slot<int> & slot) {
    m_sizeSlot = slot;
}
