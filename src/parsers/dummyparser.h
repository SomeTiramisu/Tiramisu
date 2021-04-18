#ifndef DUMMYPARSER_H
#define DUMMYPARSER_H

#include "parseutils.h"

class DummyParser {

public:
    DummyParser() {};
    ~DummyParser() {};
    cv::Mat getAt() {
        return cv::Mat();
    };
    int getSize() {
        return 1;
    }
    static bool isSupported(QUrl fn) {
        return fn.isEmpty();
    };
    QUrl getFilename() {
        return QUrl();
    };
};

#endif // DUMMYPARSER_H
