#ifndef PAGESCHEDULER_H
#define PAGESCHEDULER_H

#include "pagepreloader.h"
#include "cropscalerunner.h"
#include "utils/utils.h"

class PageScheduler
{
public:
    PageScheduler() = default;
    PageScheduler(PageScheduler&&) = default;
    PageScheduler(PagePreloader* preloader);
    PageScheduler& operator=(PageScheduler&&) = default;
    void at(const PageRequest& req, const Slot<PagePair> slot);

private:
    void seekPages(const PageRequest& req);
    std::vector<CropScaleRunner> m_pages;
    PagePreloader* m_preloader{nullptr};
    int m_imagePreload;
};

#endif // PAGESCHEDULER_H
