#include "book.h"

#include <algorithm>
#include <vector>
#include <string>
#include <QtCore>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <unarr.h>
#include "strnatcmp/strnatcmp.h"
};

Book::Book(std::string fn)
    : libarchive_book(fn), unarr_book(fn) {
    filename = fn;
    book_lib = getBookLib(fn);
    qWarning("booklib: %i", book_lib);
}

Book::~Book() {
}

int Book::getBookLib(std::string fn) {
    if (fn == "dummy")
        return DUMMY;
    if (LibarchiveBook::isSupported(fn))
        return LIBARCHIVE;
    if (UnarrBook::isSupported(fn))
        return UNARR;
    return UNSUPPORTED;
}

Page Book::getAt(int index, int width, int height) {
    if (book_lib == LIBARCHIVE)
        return libarchive_book.getAt(index);
    if (book_lib == UNARR)
        return unarr_book.getAt(index);
    return Page {cv::Mat(), 0, 0, 0, ""};

}

int Book::getSize() {
    if (book_lib == DUMMY)
        return 1;
    if (book_lib == LIBARCHIVE)
        return libarchive_book.getSize();
    if (book_lib == UNARR)
        return unarr_book.getSize();
    return 0;
}

std::string Book::getFilename() {
    return filename;
}

LibarchiveBook::LibarchiveBook(std::string fn)
{
    if (isSupported(fn)) {
        filename = fn;
        openArchive(filename);

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

void LibarchiveBook::openArchive(std::string fn) {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    //archive_read_support_format_rar(bookArchive); buggy
    archive_read_open_filename(bookArchive, fn.c_str(), 10240);
}

Page LibarchiveBook::getAt(int index) {
    int n = headers[index].index;
    openArchive(filename);
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

std::string LibarchiveBook::getFilename() {
    return filename;
}

bool LibarchiveBook::isSupported(std::string fn) {
    if (fn == "dummy")
        return false;
    struct archive *a;
    int r;
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    r = archive_read_open_filename(a, fn.c_str(), 10240);
    if (r != ARCHIVE_OK)
        return false;
    r = archive_read_free(a);
    return  true;

}

UnarrBook::UnarrBook(std::string fn)
{
    if (isSupported(fn)) {
        filename = fn;
        openArchive(filename);
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

void UnarrBook::openArchive(std::string fn) {
	bookStream = ar_open_file(fn.c_str());
    bookArchive = ar_open_rar_archive(bookStream);
}

Page UnarrBook::getAt(int index) {
    int n = headers[index].index;
    openArchive(filename);
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

std::string UnarrBook::getFilename() {
    return filename;
}

bool UnarrBook::isSupported(std::string fn) {
    if (fn == "dummy")
        return false;   
    ar_stream *s  = nullptr;    
    ar_archive *a = nullptr;
    s = ar_open_file(fn.c_str());
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

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    return r < 0;
}
