#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include "parseutils.h"
#include "dummyparser.h"
#include "libarchiveparser.h"
#include "unarrparser.h"

class Parser {

public:
    Parser() = default;
    Parser(std::filesystem::path& filename);
    Parser(std::vector<char>&);
    std::vector<char> at(int index);
    int size() const;

private:
    ParserLib getBookLib(const std::filesystem::path& fn) const;
    ParserLib getBookLib(const std::vector<char>& ramArchive) const;
    std::unique_ptr<ParserBase> m_parser;
};

#endif // PARSER_H
