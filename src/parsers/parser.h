#ifndef PARSER_H
#define PARSER_H

#include "parseutils.h"
#include "dummyparser.h"
#include "libarchiveparser.h"
#include "unarrparser.h"
#include "popplerparser.h"
#include <QMutex>

class Parser {

public:
    Parser(QUrl fn, bool toram);
    ~Parser();
    PageResponseCV getAt(int index);
    int getSize();
    QUrl getFilename();
private:
    int book_lib;
    QUrl filename;
    QMutex lock;
    int getBookLib(QUrl fn);
    DummyParser dummy_parser;
    LibarchiveParser *libarchive_parser;
    UnarrParser *unarr_parser;
    PopplerBook *poppler_parser;
};

#endif // PARSER_H
