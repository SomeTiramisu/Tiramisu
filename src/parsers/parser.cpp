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
        QByteArray ramArchive = file.readAll();
        ParserBase* parser;
        if (m_bookLib== ParserLib::Libarchive) {
            parser = new LibarchiveParser(&ramArchive);
        }
        if (m_bookLib== ParserLib::Unarr) {
            parser = new UnarrParser(&ramArchive);
        }
        m_ramExArchive.reserve(parser->size());
        for (int i=0; i<parser->size(); i++) {
            m_ramExArchive.append(parser->at(i));
        }
        file.close();
        delete parser;
    } else {
        m_isRam = false; //case of isEmpty is true
        if (m_bookLib== ParserLib::Libarchive) {
            m_libarchiveParser = new LibarchiveParser(m_filename);
        }
        if (m_bookLib== ParserLib::Unarr) {
            m_unarrParser = new UnarrParser(m_filename);
        }
    }
}

Parser::~Parser() {
    delete m_libarchiveParser;
    delete m_unarrParser;
}

ParserLib Parser::getBookLib(const QUrl& fn) const {
    if (DummyParser::isSupported(fn)) {
        return ParserLib::Dummy;
    }
    if (LibarchiveParser::isSupported(fn)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(fn)) {
        return ParserLib::Unarr;
    }
    return ParserLib::Unsupported;
}

QByteArray Parser::at(int index) {
    QMutexLocker locker(&mutex);
    if (m_isRam) {
        //QImage img = QImage::fromData(m_ramExArchive.at(index));
        //qWarning("check, %i, %i", img.width(), img.height());
        return m_ramExArchive.at(index);
    }
    if (m_bookLib== ParserLib::Dummy) {
        return m_dummyParser.at();
    }
    if (m_bookLib== ParserLib::Libarchive) {
        return m_libarchiveParser->at(index);
    }
    if (m_bookLib== ParserLib::Unarr) {
        return m_unarrParser->at(index);
    }
    return QByteArray();
}

int Parser::size() {
    if (m_isRam) {
        return m_ramExArchive.size();
    }
    if (m_bookLib== ParserLib::Dummy) {
        return m_dummyParser.size();
    }
    if (m_bookLib== ParserLib::Libarchive) {
        return m_libarchiveParser->size();
    }
    if (m_bookLib== ParserLib::Unarr) {
        return m_unarrParser->size();
    }
    return -1;
}

QUrl Parser::filename() const {
    return m_filename;
}
