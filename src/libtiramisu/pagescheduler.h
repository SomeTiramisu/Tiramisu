#ifndef PAGESCHEDULER_H
#define PAGESCHEDULER_H

#include "pagepreloader.h"
#include "utils/utils.h"

class PageScheduler
{
public:
    PageScheduler(PagePreloader* preloader);
    ~PageScheduler();
    cv::Mat getPage(PageRequest req);

private:
    void preloadPages(PageRequest req);
    void clearPages(PageRequest req);
    static void runPage(PagePreloader*, PageRequest, std::vector<PagePair>*);
    std::vector<PagePair> m_pages;
    PagePreloader* m_preloader{nullptr};
    const int m_imagePreload;
};

#endif // PAGESCHEDULER_H
