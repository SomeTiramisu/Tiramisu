#include "parser.h"
#include <QtCore>

Parser::Parser(QUrl filename, bool isRam)
    : m_bookLib(getBookLib(filename)),
      m_filename(filename),
      m_isRam(isRam)
{
    if (m_isRam && not m_filename.isEmpty()) {
        QFile file(m_filename.toLocalFile());
        file.open(QIODevice::ReadOnly);
        m_ramArchive = file.readAll();
        m_parser = new DummyParser();
        if (m_bookLib== ParserLib::Libarchive) {
            delete m_parser;
            m_parser = new LibarchiveParser(&m_ramArchive);
        }
        if (m_bookLib== ParserLib::Unarr) {
            delete m_parser;
            m_parser = new UnarrParser(&m_ramArchive);
        }
    } else {
        m_isRam = false; //case of isEmpty is true
        m_parser = new DummyParser();
        if (m_bookLib== ParserLib::Libarchive) {
            delete m_parser;
            m_parser = new LibarchiveParser(m_filename);
        }
        if (m_bookLib== ParserLib::Unarr) {
            delete m_parser;
            m_parser = new UnarrParser(m_filename);
        }
    }
}

Parser::~Parser() {
    delete m_parser;
}

ParserLib Parser::getBookLib(const QUrl& fn) const {
    if (LibarchiveParser::isSupported(fn)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(fn)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Dummy;
}

QByteArray Parser::at(int index) {
    //QMutexLocker locker(&mutex);
    return m_parser->at(index);
}

int Parser::size() const {
    return m_parser->size();
}

QUrl Parser::filename() const {
    return m_filename;
}
