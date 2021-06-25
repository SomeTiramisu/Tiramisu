#ifndef PAGESCHEDULER_H
#define PAGESCHEDULER_H

#include "cropscalerunner.h"
#include "utils/utils.h"

class PageScheduler
{
public:
    PageScheduler();
    //PageScheduler(PageScheduler&&) = default;
    //PageScheduler& operator=(PageScheduler&&) = default;
    ~PageScheduler();
    void at(const PageRequest& req);
    void connectPageSlot(const Slot<PagePair>&);
    void connectSizeSlot(const Slot<int>&);

private:
    void seekPages(const PageRequest& req);
    std::vector<CropScaleRunner> m_pages;
    int m_imagePreload;
    QThreadPool m_pool;
    Slot<PagePair> m_pageSlot;
    Slot<int> m_sizeSlot;
    Path m_filename;
    Parser m_parser;
};

#endif // PAGESCHEDULER_H
