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
    Page getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    archive *bookArchive;
    archive_entry *entry;  //TODO : inutile ici, rempkacer localement
    std::vector<header> headers;
    void openRamArchive();
    void openArchive();
    void initRamArchive();
    int size;
    QUrl filename;
    bool isRam;
    QByteArray ram_archive;
};

#endif // LIBARCHIVEPARSER_H
