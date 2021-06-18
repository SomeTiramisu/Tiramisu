#ifndef TIRAMISU_H
#define TIRAMISU_H

#include "utils/utils.h"
#include "pagepreloader.h"
#include "pagescheduler.h"

class Tiramisu
{
public:
    cv::Mat get(PageRequest req);
    int bookSize() const {return m_bookSize;}
    int preloaderProgress() const { return m_preloader.progress();}

private:
    void setFilename(const Path& filename);
    Path m_filename;
    int m_bookSize{0};
    PageScheduler m_scheduler;
    PagePreloader m_preloader;
    PageRequest m_req;
};

#endif // TIRAMISU_H
