#ifndef TIRAMISUC_H
#define TIRAMISUC_H

#include "utils/utils.h"
#include "pagescheduler.h"

extern "C" {
typedef struct {
    PageScheduler m_scheduler;
    PageRequest m_req;
    Slot<cv::Mat> m_schedulerImageSlot;
    Slot<int> m_bookSizeSlot;
    Slot<int> m_preloaderProgressSlot;
} tiramisu_t;

void tiramisu_init(tiramisu_t*);
void tiramisu_free(tiramisu_t*);
void tiramisu_get(tiramisu_t*, int index, int width, int height, char* filename, size_t length);
void tiramisu_connect_preloader_progress(tiramisu_t*, void (*slot)(int));
void tiramisu_connect_book_size(tiramisu_t*, void (*slot)(int));
void tiramisu_connect_image(tiramisu_t*, void (*slot)(char*, size_t));
};

#endif // TIRAMISUC_H
