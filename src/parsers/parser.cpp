#include "parser.h"
#include <QtCore>

Parser::Parser(QUrl fn, bool toram)
    : bookLib(getBookLib(fn)),
      filename(fn),
      isRam(toram)
{
    if (isRam) {
        initRamArchive();
        if (bookLib == ParserLib::Libarchive) {
            libarchiveParser = new LibarchiveParser(&ramArchive);
        }
        if (bookLib == ParserLib::Unarr) {
            unarrParser = new UnarrParser(&ramArchive);
        }
    } else {
        if (bookLib == ParserLib::Libarchive) {
            libarchiveParser = new LibarchiveParser(fn);
        }
        if (bookLib == ParserLib::Unarr) {
            unarrParser = new UnarrParser(fn);
        }
    }
}

Parser::~Parser() {
    delete libarchiveParser;
    delete unarrParser;
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

cv::Mat Parser::getAt(int index) {
    QMutexLocker locker(&mutex);
    if (bookLib == ParserLib::Dummy) {
        return dummyParser.getAt();
    }
    if (bookLib == ParserLib::Libarchive) {
        return libarchiveParser->getAt(index);
    }
    if (bookLib == ParserLib::Unarr) {
        return unarrParser->getAt(index);
    }
    return cv::Mat();

}

int Parser::getSize() {
    if (bookLib == ParserLib::Dummy) {
        return dummyParser.getSize();
    }
    if (bookLib == ParserLib::Libarchive) {
        return libarchiveParser->getSize();
    }
    if (bookLib == ParserLib::Unarr) {
        return unarrParser->getSize();
    }
    return -1;
}

QUrl Parser::getFilename() const {
    return filename;
}

void Parser::reset(const QUrl& fn, bool toram) {
    QMutexLocker locker(&mutex);
    if (fn==filename) {
        return;
    }
    delete libarchiveParser;
    delete unarrParser;
    Parser(fn, toram);
}

void Parser::initRamArchive() {
    QFile file(filename.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ramArchive = file.readAll();
    file.close();
}
