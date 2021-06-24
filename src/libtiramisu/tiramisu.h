#ifndef TIRAMISU_H
#define TIRAMISU_H

#include "utils/utils.h"
#include "pagepreloader.h"
#include "pagescheduler.h"

class Tiramisu
{
public:
    void get(const PageRequest& req);
    void connectPreloaderProgress(const Slot<int>& slot);
    void connectBookSize(const Slot<int>& slot);
    void connectImage(const Slot<cv::Mat>& slot);

private:
    void setFilename(const Path& filename);
    Path m_filename;
    PageScheduler m_scheduler;
    PagePreloader m_preloader;
    PageRequest m_req;
    Slot<cv::Mat> m_schedulerImageSlot;
    Slot<int> m_bookSizeSlot;
    Slot<int> m_preloaderProgressSlot;
    QThreadPool m_pool;
};

#endif // TIRAMISU_H
