#ifndef DUMMYPARSER_H
#define DUMMYPARSER_H

#include "parseutils.h"

class DummyParser {

public:
    DummyParser() {};
    ~DummyParser() {};
    cv::Mat at() {
        return cv::Mat();
    };
    int size() {
        return 1;
    }
    static bool isSupported(QUrl fn) {
        return fn.isEmpty();
    };
};

#endif // DUMMYPARSER_H
