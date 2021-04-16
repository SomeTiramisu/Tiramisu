#ifndef PARSEUTILS_H
#define PARSEUTILS_H

#include <opencv2/core.hpp>
#include <QUrl>
#include "../utils/utils.h"

extern "C" {
    #include "../strnatcmp/strnatcmp.h"
}

#define UNSUPPORTED 0
#define DUMMY 1
#define LIBARCHIVE 2
#define UNARR 3
#define POPPLER 4

struct header {
    std::string filename;
    int index;
    size_t length;
};

class ParserUtils {
public:
    static bool naturalCompare(const header &a, const header &b) {
        int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
        return r < 0;
    }
};

#endif // PARSEUTILS_H
