#ifndef TIRAMISUC_H
#define TIRAMISUC_H

#include "utils/utils.h"
#include "pagepreloader.h"
#include "pagescheduler.h"

extern "C" {

typedef
struct {
    Path filename;
    std::unique_ptr<PageScheduler> scheduler;
    std::unique_ptr<PagePreloader> preloader;
} tiramisu;

typedef
struct {
    int index;
    cv::Mat img;
} tiramisuPage;

void tiramisu_open_filename(tiramisu* t, const char* fn);
void tiramisu_get_page(tiramisu* t, tiramisuPage* p, int index, int width, int height);
void tiramisu_page_get_data(tiramisuPage* p, char* buf, size_t length);
size_t tiramisu_page_get_length(tiramisuPage* p);
int tiramisu_page_get_index(tiramisuPage* p);
int tiramisu_get_size(tiramisu* t);
int tiramisu_get_progress(tiramisu* t);
}

#endif // TIRAMISUC_H
