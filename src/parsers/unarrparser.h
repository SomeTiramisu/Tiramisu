#ifndef UNARRPARSER_H
#define UNARRPARSER_H

#include "parseutils.h"

extern "C" {
#include <unarr.h>
};

class UnarrParser {

public:
    UnarrParser(QUrl fn, bool toram);
    ~UnarrParser() {};
    PageResponseCV getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    ar_stream *bookStream;
    ar_archive *bookArchive;
    std::vector<header> headers;
    void openRamArchive();
    void openArchive();
    void initRamArchive();
    int size;
    QUrl filename;
    bool isRam;
    QByteArray ram_archive;
};

#endif // UNARRPARSER_H
