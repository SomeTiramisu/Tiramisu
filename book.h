#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
};

#define UNSUPPORTED 0
#define DUMMY 1
#define LIBARCHIVE 2
#define UNARR 3
#define POPPLER 4

struct header {
    std::string filename;
    int index;
    long long length;
};

class LibarchiveBook {

public:
    LibarchiveBook(std::string fn);
    ~LibarchiveBook();
    cv::Mat getAt(int index);
    int getSize();
    static bool isSupported(std::string fn);
    std::string getFilename();

private:
    archive *bookArchive;
    archive_entry *entry;
    std::vector<header> headers;
    void openArchive(std::string filename);
    int size;
    std::string filename;

};

class Book {

public:
    Book(std::string fn);
    ~Book();
    cv::Mat getAt(int index);
    int getSize();
    std::string getFilename();
private:
    int book_lib;
    std::string filename;
    int getBookLib(std::string fn);
    LibarchiveBook libarchive_book;
};

bool naturalCompare(const header &a, const header &b);

#endif // BOOK_H
