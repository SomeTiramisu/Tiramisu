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
    CropScaleRunner(PagePreloader*);
    void get(const PageRequest& req, const Slot<PagePair>& slot);
    void get(const PageRequest& req);
    void clear();
    CropScaleRunner& operator=(CropScaleRunner&&) = default;

private:
    void run();
    static PagePair cropScale(PagePreloader* preloader, const PageRequest& req);
    void handleCropScale(const PagePair& res);
    PageRequest m_req;
    PagePreloader* m_preloader{nullptr};
    PagePair m_res;
    std::thread m_thread;
    Slot<PagePair> m_slot;

};

#endif // CROPSCALERUNNER_H
