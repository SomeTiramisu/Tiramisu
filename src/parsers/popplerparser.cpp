#include "popplerparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>
#include <poppler/cpp/poppler-page-renderer.h>

PopplerBook::PopplerBook(QUrl fn)
{
    if (isSupported(fn)) {
        filename = fn;
        initArchive(fn);
        openArchive();
        size = bookDoc->pages();
    //qWarning("book size: %i", size);
    }
}

void PopplerBook::openArchive() {
    bookDoc = poppler::document::load_from_raw_data(ram_archive.data(), ram_archive.length());
}

void PopplerBook::initArchive(QUrl fn) {
    QFile file(fn.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ram_archive = file.readAll();
    file.close();
}

Page PopplerBook::getAt(int index) {
    openArchive();
    poppler::page *p = bookDoc->create_page(index);
    if (!p)
        return Page {cv::Mat(), 0, 0, 0, filename};
    poppler::page_renderer pr;
    pr.set_image_format(poppler::image::format_bgr24);
    //poppler::image i = pr.render_page(p, 224.0, 224.0, -1, -1, width, height, poppler::rotate_0);
    poppler::image i = pr.render_page(p, 400.0, 400.0);
    cv::Mat timg = cv::Mat(i.height(), i.width(), CV_8UC3, i.data(), i.bytes_per_row());
    cv::Mat fimg;
    timg.copyTo(fimg);
    Page r = {fimg, fimg.cols, fimg.rows, index, filename};
    return r;
}

int PopplerBook::getSize() {
    return size;
}

QUrl PopplerBook::getFilename() {
    return filename;
}

bool PopplerBook::isSupported(QUrl fn) {
    if (fn.isEmpty())
        return false;
    poppler::document *d  = nullptr;
    d = poppler::document::load_from_file(fn.toLocalFile().toStdString());
    if (!d)
        return false;
    return true;
}
