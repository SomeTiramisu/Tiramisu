#ifndef LIBARCHIVEPARSER_H
#define LIBARCHIVEPARSER_H

#include "parseutils.h"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
};

class LibarchiveParser {

public:
    LibarchiveParser(const QUrl& fn);
    LibarchiveParser(QByteArray* ramArchive);
    ~LibarchiveParser() {};
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

#endif // LIBARCHIVEPARSER_H
