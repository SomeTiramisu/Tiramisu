#include "parser.h"

Parser::Parser(QUrl fn, bool toram)
    : book_lib(getBookLib(fn)),
      filename(fn)
{
    //qWarning("Book opened: %s", fn.toLocalFile().toStdString().c_str());
    if (book_lib == ParserLib::Libarchive) {
        libarchive_parser = new LibarchiveParser(fn, toram);
    }
    if (book_lib == ParserLib::Unarr) {
        unarr_parser = new UnarrParser(fn, toram);
    }
    //if (book_lib == ParserLib::Poppler) {
    //    poppler_parser = new PopplerBook(fn);
    //}
}

Parser::~Parser() {
    delete libarchive_parser;
    delete unarr_parser;
    //delete poppler_parser;
}

ParserLib Parser::getBookLib(QUrl fn) {
    if (DummyParser::isSupported(fn)) {
        return ParserLib::Dummy;
    }
    if (LibarchiveParser::isSupported(fn)) {
        return ParserLib::Libarchive;
    }
    if (UnarrParser::isSupported(fn)) {
        return ParserLib::Unarr;
    }
    //if(PopplerBook::isSupported(fn)) Disable pdf support for the moment
    //    return ParserLib::Poppler;
    return ParserLib::Unsupported;
}

cv::Mat Parser::getAt(int index) {
    QMutexLocker locker(&mutex);
    cv::Mat ret;
    if (book_lib == ParserLib::Dummy) {
        ret = dummy_parser.getAt();
    }
    if (book_lib == ParserLib::Libarchive) {
        ret = libarchive_parser->getAt(index);
    }
    if (book_lib == ParserLib::Unarr) {
        ret = unarr_parser->getAt(index);
    }
    //if (book_lib == ParserLib::Poppler) {
    //    ret = poppler_parser->getAt(index);
    //}
    return ret;

}

int Parser::getSize() {
    if (book_lib == ParserLib::Dummy) {
        return dummy_parser.getSize();
    }
    if (book_lib == ParserLib::Libarchive) {
        return libarchive_parser->getSize();
    }
    if (book_lib == ParserLib::Unarr) {
        return unarr_parser->getSize();
    }
    //if (book_lib == ParserLib::Poppler) {
    //    return poppler_parser->getSize();
    //}
    return -1;
}

QUrl Parser::getFilename() const {
    return filename;
}

void Parser::setFilename(const QUrl& fn) {
    QMutexLocker locker(&mutex);
    if (fn==getFilename()) {
        return;
    }
    book_lib = getBookLib(fn);
    filename = fn;
    delete libarchive_parser;
    delete unarr_parser;
    //delete poppler_parser;
    if (book_lib == ParserLib::Libarchive) {
        libarchive_parser = new LibarchiveParser(fn, true);
    }
    if (book_lib == ParserLib::Unarr) {
        unarr_parser = new UnarrParser(fn, true);
    }
    //if (book_lib == ParserLib::Poppler) {
    //    poppler_parser = new PopplerBook(fn);
    //}
}
