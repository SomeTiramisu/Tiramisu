#ifndef UNARRPARSER_H
#define UNARRPARSER_H

#include "parseutils.h"

extern "C" {
#include <unarr.h>
}

class UnarrParser: public ParserBase {

public:
    UnarrParser(const std::filesystem::path& fn);
    UnarrParser(std::vector<char>& ramArchive);
    ~UnarrParser() override {};
    std::vector<char> at(int index) override;
    int size() const override;
    static bool isSupported(const std::filesystem::path& fn);
    static bool isSupported(const std::vector<char>&);

private:
    std::vector<header> m_headers;
    int m_size{0};
    std::filesystem::path m_filename;
    bool m_isRam{false};
    std::vector<char> m_ramArchive;
};

#endif // UNARRPARSER_H
