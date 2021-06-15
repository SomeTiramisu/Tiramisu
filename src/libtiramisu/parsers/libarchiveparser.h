#ifndef LIBARCHIVEPARSER_H
#define LIBARCHIVEPARSER_H

#include "parseutils.h"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
}

class LibarchiveParser: public ParserBase {

public:
    LibarchiveParser(const Path& fn);
    LibarchiveParser(ByteVect& ramArchive);
    ~LibarchiveParser() override {};
    ByteVect at(int index) const override;
    int size() const override;
    static bool isSupported(const Path& fn);
    static bool isSupported(const ByteVect&);

private:
    std::vector<header> m_headers;
    int m_size{0};
    Path m_filename;
    bool m_isRam{false};
    ByteVect m_ramArchive;
};

#endif // LIBARCHIVEPARSER_H
