#ifndef TIRAMISU_H
#define TIRAMISU_H

#include "utils/utils.h"
#include "pagescheduler.h"

class Tiramisu
{
public:
    Tiramisu();
    void get(const PageRequest& req);
    void connectPreloaderProgress(const Slot<int>& slot);
    void connectBookSize(const Slot<int>& slot);
    void connectImage(const Slot<cv::Mat>& slot);

private:
    PageScheduler m_scheduler;
    PageRequest m_req;
    Slot<cv::Mat> m_schedulerImageSlot;
    Slot<int> m_bookSizeSlot;
    Slot<int> m_preloaderProgressSlot;
};

#endif // TIRAMISU_H
