#ifndef CROPSCALERUNNER_H
#define CROPSCALERUNNER_H

#include <thread>
#include "pagepreloader.h"
#include "utils/utils.h"

class CropScaleRunner
{
public:
    CropScaleRunner() = default;
    CropScaleRunner(CropScaleRunner&& other) = default;
    CropScaleRunner(PagePreloader*, QThreadPool* pool);
    void get(const PageRequest& req, const Slot<PagePair>& slot);
    void get(const PageRequest& req);
    void clear();
    CropScaleRunner& operator=(CropScaleRunner&&) = default;

private:
    void run();
    static PagePair cropScale(const PngPair& p, const PageRequest& req);
    PageRequest m_req;
    PagePreloader* m_preloader{nullptr};
    QThreadPool* m_pool{nullptr};
    PagePair m_res;
    Slot<PagePair> m_slot;

};

#endif // CROPSCALERUNNER_H
