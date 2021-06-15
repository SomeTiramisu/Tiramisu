#ifndef UNARRPARSER_H
#define UNARRPARSER_H

#include "parseutils.h"

extern "C" {
#include <unarr.h>
}

class UnarrParser: public ParserBase {

public:
    UnarrParser(const Path& fn);
    UnarrParser(ByteVect& ramArchive);
    ~UnarrParser() override {};
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

#endif // UNARRPARSER_H
