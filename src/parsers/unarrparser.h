#ifndef UNARRPARSER_H
#define UNARRPARSER_H

#include "parseutils.h"

extern "C" {
#include <unarr.h>
};

class UnarrParser {

public:
    UnarrParser(const QUrl& fn);
    UnarrParser(QByteArray* ramArchive);
    ~UnarrParser() {};
    cv::Mat at(int index);
    int size() const;
    static bool isSupported(const QUrl& fn);
    bool isSupported() const;

private:
    std::vector<header> m_headers;
    int m_size{0};
    QUrl m_filename;
    bool m_isRam{false};
    QByteArray* m_ramArchive;
};

#endif // UNARRPARSER_H
