#ifndef DUMMYPARSER_H
#define DUMMYPARSER_H

#include "parseutils.h"

class DummyParser: public ParserBase {

public:
    DummyParser() {};
    ~DummyParser() override {};
    QByteArray at(int index) override {
        Q_UNUSED(index)
        return QByteArray();
    }
    int size() const override {
        return 1;
    }
    static bool isSupported(QUrl fn) {
        Q_UNUSED(fn)
        return true;
    }

    bool isSupported() const override {
        return true;
    }
};

#endif // DUMMYPARSER_H
