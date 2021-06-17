#include "tiramisuc.h"

extern "C" {

void tiramisu_open_filename(tiramisu* t, const char* fn) {
    t->preloader = std::make_unique<PagePreloader>(fn);
    t->scheduler = std::make_unique<PageScheduler>(t->preloader.get());
}

void tiramisu_get_page(tiramisu* t, tiramisuPage* p, int index, int width, int height) {
    p->index = index;
    p->img = t->scheduler->getPage(PageRequest{width, height, index, t->filename});
}

void tiramisu_page_get_data(tiramisuPage* p, char* buf, size_t length) {
    std::copy(p->img.data, p->img.data+length, buf);
}

size_t tiramisu_page_get_length(tiramisuPage* p) {
    return p->img.total()*p->img.elemSize();
}

int tiramisu_page_get_index(tiramisuPage* p) {
    return p->index;
}

int tiramisu_get_size(tiramisu* t) {
    return t->preloader->size();
}

int tiramisu_get_progress(tiramisu* t) {
    return t->preloader->progress();
}

}
