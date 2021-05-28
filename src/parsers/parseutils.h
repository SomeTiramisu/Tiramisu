#ifndef PARSEUTILS_H
#define PARSEUTILS_H

#include <QUrl>
#include "../utils/utils.h"

extern "C" {
#include "../strnatcmp/strnatcmp.h"
}

enum ParserLib {
    Unsupported,
    Dummy,
    Libarchive,
    Unarr,
    Poppler
};

struct header {
    std::string filename;
    int index;
    size_t length;
};

class ParserBase {
public:
    virtual ~ParserBase() {};
    virtual QByteArray at(int index) = 0;
    virtual int size() const = 0;
    virtual bool isSupported() const = 0;
};

class ParserUtils {
public:
    static bool naturalCompare(const header &a, const header &b) {
        int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
        return r < 0;
    }
};

#endif // PARSEUTILS_H
