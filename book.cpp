#include "book.h"

#include <algorithm>
#include <vector>
#include <string>
#include <QtCore>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include "strnatcmp/strnatcmp.h"
};

//#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"

Book::Book()
{
    openArchive(ARCHIVE_FILENAME);

    int i = 0;
    while (archive_read_next_header(bookArchive, &entry) == ARCHIVE_OK) {
        header h = {
            .filename = std::string(archive_entry_pathname(entry)),
            .index = i
        };
        if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
            headers.push_back(h);
        }
        archive_read_data_skip(bookArchive);
        i++;
    }
    size = i;
    archive_read_free(bookArchive);
    /*
    for (std::vector<header>::iterator it=headers.begin(); it !=headers.end(); ++it) {
        qWarning("%s", it->filename.c_str());
    }
    */
    std::sort(headers.begin(), headers.end(), naturalCompare);

    //cindex = settings.value("currentPage", 0).toInt();
    cindex = 0;
    loadBufAt(cindex);


}

Book::~Book()
{
    delete[] buf;
    //settings.setValue("currentPage", cindex);
}

char* Book::getCurrent()
{
    int n = headers[cindex].index;
    loadBufAt(n);
    return buf;
}

char* Book::getNext()
{
    //qWarning("Loading %s", headers[cindex].filename.c_str());
    if (cindex < size) {
        cindex++;
    }
    int n = headers[cindex].index;
    loadBufAt(n);
    return buf;
}

char* Book::getPrevious()
{
    if (cindex > 0) {
        cindex--;
    }
    int n = headers[cindex].index;
    loadBufAt(n);
    return buf;
}


char* Book::getAt(int index)
{
    if (index > 0 && index < size) {
        cindex = index;
    }
    return getCurrent();
}


unsigned int Book::getLength()
{
    return length;
}

void Book::openArchive(std::string filename) {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    archive_read_support_format_rar(bookArchive);
    archive_read_open_filename(bookArchive, filename.c_str(), 10240);
}

void Book::loadBufAt(int n) {
    openArchive(ARCHIVE_FILENAME);
    for (int i=0; i<=n; i++) {
        archive_read_next_header(bookArchive, &entry);
    }
    length = archive_entry_size(entry);
    //delete[] buf; deleted after Image creation
    buf = new char[length];
    archive_read_data(bookArchive, buf, length);
    archive_read_free(bookArchive);
}

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    if (r < 0) {
        return true;
    }
    return false;
}
