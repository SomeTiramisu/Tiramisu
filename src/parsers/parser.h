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
    void reset(const QUrl& fn, bool toram);
private:
    void initRamArchive();
    ParserLib bookLib{ParserLib::Dummy};
    QUrl filename;
    inline static QMutex mutex;
    ParserLib getBookLib(const QUrl& fn) const;
    QByteArray ramArchive;
    bool isRam{false};
    DummyParser dummyParser;
    LibarchiveParser *libarchiveParser{nullptr};
    UnarrParser *unarrParser{nullptr};
};

#endif // PARSER_H
