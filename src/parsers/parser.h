#ifndef PARSER_H
#define PARSER_H

#include "parseutils.h"
#include "dummyparser.h"
#include "libarchiveparser.h"
#include "unarrparser.h"
//#include "popplerparser.h"
#include <QMutex>

class Parser {

public:
    Parser(QUrl fn = QUrl(), bool toram = false);
    ~Parser();
    cv::Mat getAt(int index);
    int getSize();
    QUrl getFilename() const;
    void setFilename(const QUrl& fn);
private:
    ParserLib book_lib{ParserLib::Dummy};
    QUrl filename;
    inline static QMutex mutex;
    ParserLib getBookLib(QUrl fn);
    DummyParser dummy_parser;
    LibarchiveParser *libarchive_parser{nullptr};
    UnarrParser *unarr_parser{nullptr};
    //PopplerBook *poppler_parser{nullptr};
};

#endif // PARSER_H
