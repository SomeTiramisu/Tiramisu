#ifndef PARSER_H
#define PARSER_H

#include "parseutils.h"
#include "dummyparser.h"
#include "libarchiveparser.h"
#include "unarrparser.h"
#include <QMutex>

class Parser {

public:
    Parser(QUrl filename = QUrl(), bool isRam = false);
    ~Parser();
    QByteArray at(int index);
    int size() const;
    QUrl filename() const;

private:
    ParserLib m_bookLib{ParserLib::Dummy};
    QUrl m_filename;
    inline static QMutex mutex;
    ParserLib getBookLib(const QUrl& fn) const;
    QVector<QByteArray> m_ramExArchive;
    bool m_isRam{false};
    ParserBase *m_parser{nullptr};
};

#endif // PARSER_H
