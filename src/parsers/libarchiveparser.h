#ifndef LIBARCHIVEPARSER_H
#define LIBARCHIVEPARSER_H

#include "parseutils.h"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
};

class LibarchiveParser {

public:
    LibarchiveParser(QUrl fn, bool toram);
    ~LibarchiveParser() {};
    cv::Mat getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    archive *bookArchive{nullptr};
    std::vector<header> headers;
    void openRamArchive();
    void openArchive();
    void initRamArchive();
    int size{0};
    QUrl filename;
    bool isRam{false};
    QByteArray ram_archive;
};

#endif // LIBARCHIVEPARSER_H
