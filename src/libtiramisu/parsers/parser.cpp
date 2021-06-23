#include "parser.h"
#include <fstream>
#include <iterator>

Parser::Parser(const Path& filename) {
    if (not filename.empty()) {
        ParserLib bookLib = getBookLib(filename);
        m_parser = std::make_unique<DummyParser>();
        if (bookLib == ParserLib::Libarchive) {
            m_parser = std::make_unique<LibarchiveParser>(filename);
        }
        if (bookLib == ParserLib::Unarr) {
            m_parser = std::make_unique<UnarrParser>(filename);
        }
    }
}

Parser::Parser(const ByteVect& ramArchive) {
    ParserLib bookLib = getBookLib(ramArchive);
    m_parser = std::make_unique<DummyParser>();
    if (bookLib == ParserLib::Libarchive) {
        m_parser = std::make_unique<LibarchiveParser>(ramArchive);
    }
    if (bookLib == ParserLib::Unarr) {
        m_parser = std::make_unique<UnarrParser>(ramArchive);
    }
}

ParserLib Parser::getBookLib(const Path& fn) const {
    if (LibarchiveParser::isSupported(fn)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(fn)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Dummy;
}

ParserLib Parser::getBookLib(const ByteVect& ramArchive) const {
    if (LibarchiveParser::isSupported(ramArchive)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(ramArchive)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Dummy;
}

ByteVect Parser::at(int index) const {
    return m_parser->at(index);
}

int Parser::size() const {
    return m_parser->size();
}
