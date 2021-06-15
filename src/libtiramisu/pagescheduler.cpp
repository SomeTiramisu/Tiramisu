#include "pagescheduler.h"

#include "utils/imageproc.h"
#include "parsers/parser.h"

PageScheduler::PageScheduler(PagePreloader* preloader)
    : m_preloader(preloader),
      m_imagePreload(20)
{
    m_pages.resize(m_preloader->size());
    qWarning("scheduler created");
}

PageScheduler::~PageScheduler() {
    qWarning("scheduler deleted");
}

cv::Mat PageScheduler::getPage(PageRequest req) {
    int index = req.index();
    int book_size = m_preloader->size();
    if (index<0 || index >= book_size) {
        return cv::Mat();
    }
    if (m_pages.at(index).req.valid() and not m_pages.at(index).img.empty()) {
        qWarning("Controller: already reviced %i", req.index());
        return m_pages.at(index).img;
    } else if (m_pages.at(index).req.valid()) {
        qWarning("Controller: already requested, rerun local %i", req.index());
        runPage(m_preloader, req, &m_pages);
    } else {
        qWarning("Controller: running locally %i", req.index());
        runPage(m_preloader, req, &m_pages);
    }
    preloadPages(req);
    clearPages(req);
    return m_pages.at(index).img;
}

void PageScheduler::preloadPages(PageRequest req) {
    int index = req.index();
    int book_size = m_preloader->size();
    for (int i=1; i<m_imagePreload; i++) {
        PageRequest preq = req.addIndex(i);
        PageRequest mreq = req.addIndex(-i);
        if (index+i<book_size and not m_pages.at(preq.index()).req.valid()) {
            //qWarning("%i", preq.index());
            runPage(m_preloader, preq, &m_pages);
            //runLocalPage(preq);
        }
        if (index-i>=0 and not m_pages.at(mreq.index()).req.valid()) {
            //qWarning("%i", mreq.index());
            runPage(m_preloader, mreq, &m_pages);
            //runLocalPage(mreq);
        }
    }
}

void PageScheduler::clearPages(PageRequest req) {
    for (int i = 0; i<m_pages.size(); i++) {
        PagePair& p = m_pages.at(i);
        if(not p.req.isLike(req) || not p.req.isInRange(req, m_imagePreload)) {
            m_pages.at(i) = PagePair();
        }
    }
}

void PageScheduler::runPage(PagePreloader* preloader, PageRequest req, std::vector<PagePair>* pages) {
    PngPair pair = preloader->at(req.index());
    cv::Mat img = ImageProc::fromVect(pair.png);
    if (not img.empty()) {
        ImageProc::cropScaleProcess(img, img, pair.roi, req.width(), req.height());
    }
    qWarning("CropScaleRunnable: runningLocal: %i, (%i, %i) orig: (%i %i)", req.index(), req.width(), req.height(), img.cols, img.rows);
    pages->at(req.index()) = PagePair{img, req};
}
