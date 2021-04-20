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
    cv::Mat getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    ar_stream *bookStream{nullptr};
    ar_archive *bookArchive{nullptr};
    std::vector<header> headers;
    void openRamArchive();
    void openArchive();
    void initRamArchive();
    int size{0};
    QUrl filename;
    bool isRam{false};
    QByteArray ram_archive;
};

#endif // UNARRPARSER_H
