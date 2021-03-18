#include "book.h"

//#include <algorithm>
//#include <vector>
//#include <string>
#include <QtCore>
//#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <poppler/cpp/poppler-document.h>
//#include <poppler/cpp/poppler-page.h>
//#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-page-renderer.h>

extern "C" {
//#include <archive.h>
//#include <archive_entry.h>
//#include <unarr.h>
#include "strnatcmp/strnatcmp.h"
};

Book::Book(QUrl fn)
    : book_lib(getBookLib(fn)),
      filename(fn),
      libarchive_book(nullptr),
      unarr_book(nullptr),
      poppler_book(nullptr)
{
    //qWarning("booklib: %i", book_lib);
    if (book_lib == LIBARCHIVE)
        libarchive_book = new LibarchiveBook(fn);
    if (book_lib == UNARR)
        unarr_book = new UnarrBook(fn);
    if (book_lib == POPPLER)
        poppler_book = new PopplerBook(fn);
}

Book::~Book() {
    delete libarchive_book;
    delete unarr_book;
    delete poppler_book;
}

int Book::getBookLib(QUrl fn) {
    if (fn.isEmpty())
        return DUMMY;
    if (LibarchiveBook::isSupported(fn))
        return LIBARCHIVE;
    if (UnarrBook::isSupported(fn))
        return UNARR;
    if(PopplerBook::isSupported(fn))
        return POPPLER;
    return UNSUPPORTED;
}

Page Book::getAt(int index, int width, int height) {
    if (book_lib == LIBARCHIVE)
        return libarchive_book->getAt(index);
    if (book_lib == UNARR)
        return unarr_book->getAt(index);
    if (book_lib == POPPLER)
        return poppler_book->getAt(index, width, height);
    return Page {cv::Mat(), 0, 0, 0, QUrl()};

}

int Book::getSize() {
    if (book_lib == DUMMY)
        return 1;
    if (book_lib == LIBARCHIVE)
        return libarchive_book->getSize();
    if (book_lib == UNARR)
        return unarr_book->getSize();
    if (book_lib == POPPLER)
        return poppler_book->getSize();
    return 0;
}

QUrl Book::getFilename() {
    return filename;
}

LibarchiveBook::LibarchiveBook(QUrl fn)
{
    if (isSupported(fn)) {
        filename = fn;
        initArchive(filename);
        openArchive();

        int i = 0;
        size = 0;
        while (archive_read_next_header(bookArchive, &entry) == ARCHIVE_OK) {
            header h = {
                .filename = std::string(archive_entry_pathname(entry)),
                .index = i,
                .length = static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                headers.push_back(h);
                size++;
            }
            archive_read_data_skip(bookArchive);
            i++;
        }
    qWarning("book size: %i", size);
    archive_read_free(bookArchive);
    /*
    for (std::vector<header>::iterator it=headers.begin(); it !=headers.end(); ++it) {
        qWarning("%s", it->filename.c_str());
    }
    */
    std::sort(headers.begin(), headers.end(), naturalCompare);
    }
}

LibarchiveBook::~LibarchiveBook()
{
}

void LibarchiveBook::openArchive() {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    //archive_read_support_format_rar(bookArchive); buggy
    archive_read_open_memory(bookArchive, ram_archive.data(), ram_archive.length()); //may be incorrect
}

void LibarchiveBook::initArchive(QUrl fn) {
    QFile file(fn.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ram_archive = file.readAll();
    file.close();
}

Page LibarchiveBook::getAt(int index) {
    int n = headers[index].index;
    openArchive();
    for (int i=0; i<=n; i++) {
        archive_read_next_header(bookArchive, &entry);
    }
    size_t length = headers[index].length;
    //delete[] buf; deleted after Image creation
    char* buf = new char[length]; //TODO: use voud buffer
    archive_read_data(bookArchive, buf, length);
    archive_read_free(bookArchive);
    cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    Page p = {img, img.cols, img.rows, index, filename};
    delete[] buf;
    return p;
}

int LibarchiveBook::getSize() {
    return size;
}

QUrl LibarchiveBook::getFilename() {
    return filename;
}

bool LibarchiveBook::isSupported(QUrl fn) {
    if (fn.isEmpty())
        return false;
    struct archive *a;
    int r;
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    r = archive_read_open_filename(a, fn.toLocalFile().toStdString().c_str(), 10240);
    if (r != ARCHIVE_OK)
        return false;
    r = archive_read_free(a);
    return  true;

}

UnarrBook::UnarrBook(QUrl fn)
{
    if (isSupported(fn)) {
        filename = fn;
        initArchive(filename);
        openArchive();

        int i = 0;
        size = 0;
        while (ar_parse_entry(bookArchive)) {
            header h = {
                .filename = std::string(ar_entry_get_name(bookArchive)), //may need a *
                .index = i,
                .length = ar_entry_get_size(bookArchive)
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                headers.push_back(h);
                size++;
            }
            i++;
        }
    qWarning("book size: %i", size);
    ar_close_archive(bookArchive);
    ar_close(bookStream);
 
    std::sort(headers.begin(), headers.end(), naturalCompare);
    }
}

UnarrBook::~UnarrBook()
{
}

void UnarrBook::openArchive() {
    bookStream = ar_open_memory(ram_archive.data(), ram_archive.length());
    bookArchive = ar_open_rar_archive(bookStream);
}

void UnarrBook::initArchive(QUrl fn) {
    QFile file(fn.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ram_archive = file.readAll();
    file.close();
}

Page UnarrBook::getAt(int index) {
    int n = headers[index].index;
    openArchive();
    for (int i=0; i<=n; i++) {
        ar_parse_entry(bookArchive);
    }
    size_t length = headers[index].length;
    //delete[] buf; deleted after Image creation
    char* buf = new char[length];
    ar_entry_uncompress(bookArchive, buf, length);
    ar_close_archive(bookArchive);
    ar_close(bookStream);
    cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    Page p = {img, img.cols, img.rows, index, filename};
    delete[] buf;
    return p;
}

int UnarrBook::getSize() {
    return size;
}

QUrl UnarrBook::getFilename() {
    return filename;
}

bool UnarrBook::isSupported(QUrl fn) {
    if (fn.isEmpty())
        return false;   
    ar_stream *s  = nullptr;    
    ar_archive *a = nullptr;
    s = ar_open_file(fn.toLocalFile().toStdString().c_str());
    if (!s)
        return false;
    a = ar_open_rar_archive(s);
    if (!a) {
        ar_close(s);
        return false;
        }
    ar_close_archive(a);
    ar_close(s);
    return true;

}

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

PopplerBook::~PopplerBook()
{
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

Page PopplerBook::getAt(int index, int width, int height) {
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

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    return r < 0;
}
