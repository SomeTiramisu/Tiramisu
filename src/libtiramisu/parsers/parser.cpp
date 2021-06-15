#include "parser.h"
#include <fstream>
#include <iterator>

Parser::Parser(std::filesystem::path& filename) {
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

Parser::Parser(std::vector<char>& ramArchive) {
    ParserLib bookLib = getBookLib(ramArchive);
    m_parser = std::make_unique<DummyParser>();
    if (bookLib == ParserLib::Libarchive) {
        m_parser = std::make_unique<LibarchiveParser>(ramArchive);
    }
    if (bookLib == ParserLib::Unarr) {
        m_parser = std::make_unique<UnarrParser>(ramArchive);
    }
}

ParserLib Parser::getBookLib(const std::filesystem::path& fn) const {
    if (LibarchiveParser::isSupported(fn)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(fn)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Dummy;
}

ParserLib Parser::getBookLib(const std::vector<char>& ramArchive) const {
    if (LibarchiveParser::isSupported(ramArchive)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(ramArchive)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Dummy;
}

std::vector<char> Parser::at(int index) {
    return m_parser->at(index);
}

int Parser::size() const {
    return m_parser->size();
}
