#include "tiramisuc.h"

void tiramisu_init(tiramisu_t* t) {
    t = new tiramisu_t;
    t->m_scheduler.connectPageSlot([t](const PagePair& res){
        if(t->m_req==res.req) {
            t->m_schedulerImageSlot(res.img);
        }
    });
    t->m_scheduler.connectSizeSlot([t](const int& res){
        t->m_bookSizeSlot(res);
    });
}

void tiramisu_free(tiramisu_t* t) {
    delete t;
}

void tiramisu_get(tiramisu_t* t, int index, int width, int height, char* filename, size_t length) {
    PageRequest req{index, width, height, Path(filename, length)};
    if(req.filename.empty()) {
        t->m_schedulerImageSlot(cv::Mat());
        return;
    }
    t->m_req = req;
    t->m_scheduler.at(req);
}
void tiramisu_connect_preloader_progress(tiramisu_t* t, void (*slot)(int)) {
    t->m_preloaderProgressSlot = slot;
}
void tiramisu_connect_book_size(tiramisu_t* t, void (*slot)(int)) {
    t->m_bookSizeSlot = slot;
}
void tiramisu_connect_image(tiramisu_t* t, void (*slot)(char*, size_t)) {
    t->m_schedulerImageSlot = [slot](const cv::Mat& res){
        (*slot)(reinterpret_cast<char*>(res.data), res.rows*res.cols*res.elemSize());
    };
}
