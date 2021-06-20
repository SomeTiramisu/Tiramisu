#ifndef CROPSCALERUNNER_H
#define CROPSCALERUNNER_H

#include <future>
#include "pagepreloader.h"
#include "utils/utils.h"

class CropScaleRunner
{
public:
    CropScaleRunner() = default;
    CropScaleRunner(CropScaleRunner&& other) = default;
    CropScaleRunner(PagePreloader*);
    void run(const PageRequest& req);
    PagePair get(const PageRequest& req);
    void clear();
    CropScaleRunner& operator=(CropScaleRunner&&) = default;
private:
    static PagePair cropScale(PagePreloader* preloader, const PageRequest& req);
    PageRequest m_req;
    PagePreloader* m_preloader{nullptr};
    PagePair m_res;
    std::future<PagePair> m_future;

};

#endif // CROPSCALERUNNER_H
