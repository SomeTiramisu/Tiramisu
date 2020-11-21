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

Book::Book()
{
    openArchive("/storage/emulated/0/b.cbr");

    int i = 0;
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        header h = {
            .filename = std::string(archive_entry_pathname(entry)),
            .index = i
        };
        headers.push_back(h);
        archive_read_data_skip(archive);
        i++;
    }
    archive_read_free(archive);
    std::sort(headers.begin(), headers.end(), naturalCompare);

    loadBufAt(cindex);


}

Book::~Book()
{
    delete[] buf;
}

char* Book::getCurrent()
{
    int n = headers[cindex].index;
    loadBufAt(n);
    return buf;
}

char* Book::getNext()
{
    int n = headers[++cindex].index;
    qWarning("loading %s", headers[cindex].filename.c_str());
    loadBufAt(n);
    return buf;
}

char* Book::getPrevious()
{
    int n = headers[--cindex].index;
    loadBufAt(n);
    return buf;
}


unsigned int Book::getLength()
{
    return length;
}

void Book::openArchive(std::string filename) {
    archive = archive_read_new();
    archive_read_support_filter_all(archive);
    archive_read_support_format_zip(archive);
    archive_read_support_format_rar(archive);
    archive_read_open_filename(archive, filename.c_str(), 10240);
}

void Book::loadBufAt(int n) {
    openArchive("/storage/emulated/0/b.cbr");
    for (int i=0; i<=n; i++) {
        archive_read_next_header(archive, &entry);
    }
    length = archive_entry_size(entry);
    delete[] buf;
    buf = new char[length];
    archive_read_data(archive, buf, length);
    archive_read_free(archive);
}

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    if (r < 0) {
        return true;
    }
    return false;
}
