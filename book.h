#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <QUrl>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <unarr.h>
};

#define UNSUPPORTED 0
#define DUMMY 1
#define LIBARCHIVE 2
#define UNARR 3
#define POPPLER 4

struct header {
    std::string filename;
    int index;
    size_t length;
};

struct Page {
    cv::Mat img;
    int width;
    int height;
    int index;
    QUrl book_filename;
    QUrl bg_filename;
};

class LibarchiveBook {

public:
    LibarchiveBook(QUrl fn);
    ~LibarchiveBook();
    Page getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    archive *bookArchive;
    archive_entry *entry;  //TODO : inutile ici, rempkacer localement
    std::vector<header> headers;
    void openArchive(QUrl fn);
    int size;
    QUrl filename;

};

class UnarrBook {

public:
    UnarrBook(QUrl fn);
    ~UnarrBook();
    Page getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    ar_stream *bookStream;
    ar_archive *bookArchive;
    std::vector<header> headers;
    void openArchive(QUrl fn);
    int size;
    QUrl filename;

};

class Book {

public:
    Book(QUrl fn);
    ~Book();
    Page getAt(int index, int width, int height);
    int getSize();
    QUrl getFilename();
private:
    int book_lib;
    QUrl filename;
    int getBookLib(QUrl fn);
    LibarchiveBook libarchive_book;
    UnarrBook unarr_book;
};

bool naturalCompare(const header &a, const header &b);

#endif // BOOK_H
