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
    Parser(Path& filename);
    Parser(ByteVect&);
    ByteVect at(int index) const;
    int size() const;

private:
    ParserLib getBookLib(const Path& fn) const;
    ParserLib getBookLib(const ByteVect& ramArchive) const;
    std::unique_ptr<ParserBase> m_parser;
};

#endif // PARSER_H
