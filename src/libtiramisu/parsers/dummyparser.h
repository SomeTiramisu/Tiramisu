#ifndef DUMMYPARSER_H
#define DUMMYPARSER_H

#include "parseutils.h"

class DummyParser: public ParserBase {

public:
    DummyParser() {};
    ~DummyParser() override {};
    std::vector<char> at(int index) override {
        Q_UNUSED(index)
        return std::vector<char>();
    }
    int size() const override {
        return 1;
    }
};

#endif // DUMMYPARSER_H
