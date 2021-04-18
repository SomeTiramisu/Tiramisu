#include "parser.h"

Parser::Parser(QUrl fn, bool toram)
    : book_lib(getBookLib(fn)),
      filename(fn),
      libarchive_parser(nullptr),
      unarr_parser(nullptr),
      poppler_parser(nullptr)
{
    //qWarning("Book opened: %s", fn.toLocalFile().toStdString().c_str());
    if (book_lib == LIBARCHIVE) {
        libarchive_parser = new LibarchiveParser(fn, toram);
    }
    if (book_lib == UNARR) {
        unarr_parser = new UnarrParser(fn, toram);
    }
    if (book_lib == POPPLER) {
        poppler_parser = new PopplerBook(fn);
    }
}

Parser::~Parser() {
    delete libarchive_parser;
    delete unarr_parser;
    delete poppler_parser;
}

int Parser::getBookLib(QUrl fn) {
    if (DummyParser::isSupported(fn)) {
        return DUMMY;
    }
    if (LibarchiveParser::isSupported(fn)) {
        return LIBARCHIVE;
    }
    if (UnarrParser::isSupported(fn)) {
        return UNARR;
    }
    //if(PopplerBook::isSupported(fn)) Disable pdf support for the moment
    //    return POPPLER;
    return UNSUPPORTED;
}

PageResponseCV Parser::getAt(int index) {
    lock.lock();
    PageResponseCV ret;
    if (book_lib == DUMMY) {
        return dummy_parser.getAt();
    }
    if (book_lib == LIBARCHIVE) {
        ret = libarchive_parser->getAt(index);
    }
    if (book_lib == UNARR) {
        ret = unarr_parser->getAt(index);
    }
    if (book_lib == POPPLER) {
        ret = poppler_parser->getAt(index);
    }
    lock.unlock();
    return ret;

}

int Parser::getSize() {
    if (book_lib == DUMMY) {
        return dummy_parser.getSize();
    }
    if (book_lib == LIBARCHIVE) {
        return libarchive_parser->getSize();
    }
    if (book_lib == UNARR) {
        return unarr_parser->getSize();
    }
    if (book_lib == POPPLER) {
        return poppler_parser->getSize();
    }
    return -1;
}

QUrl Parser::getFilename() {
    return filename;
}
