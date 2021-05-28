#ifndef DUMMYPARSER_H
#define DUMMYPARSER_H

#include "parseutils.h"

class DummyParser {

public:
    DummyParser() {};
    ~DummyParser() {};
    QByteArray at() {
        return QByteArray();
    }
    int size() const {
        return 1;
    }
    static bool isSupported(QUrl fn) {
        return fn.isEmpty();
    }
};

#endif // DUMMYPARSER_H
