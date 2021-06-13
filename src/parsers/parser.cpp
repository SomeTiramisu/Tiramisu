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
        ParserBase* parser = new DummyParser();
        if (m_bookLib== ParserLib::Libarchive) {
            delete parser;
            parser = new LibarchiveParser(&ramArchive);
        }
        if (m_bookLib== ParserLib::Unarr) {
            delete parser;
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
    QMutexLocker locker(&mutex);
    if (m_isRam) {
        //QImage img = QImage::fromData(m_ramExArchive.at(index));
        //qWarning("check, %i, %i", img.width(), img.height());
        return m_ramExArchive.at(index);
    }
    return m_parser->at(index);
}

int Parser::size() const {
    if (m_isRam) {
        return m_ramExArchive.size();
    }
    return m_parser->size();
}

QUrl Parser::filename() const {
    return m_filename;
}
