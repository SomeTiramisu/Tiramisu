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


Book::Book(std::string fn)
{
    filename = fn;
    openArchive(filename);

    int i = 0;
    size = 0;
    while (archive_read_next_header(bookArchive, &entry) == ARCHIVE_OK) {
        header h = {
            .filename = std::string(archive_entry_pathname(entry)),
            .index = i,
            .length = archive_entry_size(entry)
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

Book::~Book()
{
}


long long Book::getLength(int index)
{
    return headers[index].length;
}

void Book::openArchive(std::string filename) {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    //archive_read_support_format_rar(bookArchive); buggy
    archive_read_open_filename(bookArchive, filename.c_str(), 10240);
}

char* Book::getAt(int index) {
    int n= headers[index].index;
    openArchive(filename);
    for (int i=0; i<=n; i++) {
        archive_read_next_header(bookArchive, &entry);
    }
    long long length = headers[index].length;
    //delete[] buf; deleted after Image creation
    char* buf = new char[length];
    archive_read_data(bookArchive, buf, length);
    archive_read_free(bookArchive);
    return buf;
}

int Book::getSize() {
    return size;
}

std::string Book::getFilename() {
    return filename;
}

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    return r < 0;
}
