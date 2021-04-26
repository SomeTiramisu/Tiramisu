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
    Parser(QUrl filename = QUrl(), bool isRam = false);
    ~Parser();
    cv::Mat at(int index);
    int size();
    QUrl filename() const;
    void reset(const QUrl& filename, bool isRam);
private:
    void initRamArchive();
    ParserLib m_bookLib{ParserLib::Dummy};
    QUrl m_filename;
    inline static QMutex mutex;
    ParserLib getBookLib(const QUrl& fn) const;
    QByteArray m_ramArchive;
    bool m_isRam{false};
    DummyParser m_dummyParser;
    LibarchiveParser *m_libarchiveParser{nullptr};
    UnarrParser *m_unarrParser{nullptr};
};

#endif // PARSER_H
