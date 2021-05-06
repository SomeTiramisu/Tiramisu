#include "parser.h"
#include <QtCore>

Parser::Parser(QUrl filename, bool isRam)
    : m_bookLib(getBookLib(filename)),
      m_filename(filename),
      m_isRam(isRam)
{
    if (m_isRam) {
        initRamArchive();
        if (m_bookLib== ParserLib::Libarchive) {
            m_libarchiveParser = new LibarchiveParser(&m_ramArchive);
        }
        if (m_bookLib== ParserLib::Unarr) {
            m_unarrParser = new UnarrParser(&m_ramArchive);
        }
    } else {
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

cv::Mat Parser::at(int index) {
    QMutexLocker locker(&mutex);
    if (m_bookLib== ParserLib::Dummy) {
        return m_dummyParser.at();
    }
    if (m_bookLib== ParserLib::Libarchive) {
        return m_libarchiveParser->at(index);
    }
    if (m_bookLib== ParserLib::Unarr) {
        return m_unarrParser->at(index);
    }
    return cv::Mat();

}

int Parser::size() {
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

void Parser::reset(const QUrl& filename, bool isRam) {
    QMutexLocker locker(&mutex);
    delete m_libarchiveParser;
    delete m_unarrParser;
    m_bookLib= getBookLib(filename);
    m_filename = filename;
    m_isRam = isRam;
    if (m_isRam) {
        initRamArchive();
        if (m_bookLib== ParserLib::Libarchive) {
            m_libarchiveParser = new LibarchiveParser(&m_ramArchive);
        }
        if (m_bookLib== ParserLib::Unarr) {
            m_unarrParser = new UnarrParser(&m_ramArchive);
        }
    } else {
        if (m_bookLib== ParserLib::Libarchive) {
            m_libarchiveParser = new LibarchiveParser(m_filename);
        }
        if (m_bookLib== ParserLib::Unarr) {
            m_unarrParser = new UnarrParser(m_filename);
        }
    }
}

void Parser::tryReset(const QUrl &filename, bool isRam) {
    if (filename==m_filename) {
        return;
    }
    reset(filename, isRam);
}

void Parser::initRamArchive() {
    if (m_filename.isEmpty()) {
        m_isRam = false;
        return;
    }
    QFile file(m_filename.toLocalFile());
    file.open(QIODevice::ReadOnly);
    m_ramArchive = file.readAll();
    file.close();
}
