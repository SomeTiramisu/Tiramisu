#ifndef LIBARCHIVEPARSER_H
#define LIBARCHIVEPARSER_H

#include "parseutils.h"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
}

class LibarchiveParser: public ParserBase {

public:
    LibarchiveParser(const std::filesystem::path& fn);
    LibarchiveParser(std::vector<char>& ramArchive);
    ~LibarchiveParser() override {};
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

#endif // LIBARCHIVEPARSER_H
