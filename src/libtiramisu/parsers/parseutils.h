#ifndef PARSEUTILS_H
#define PARSEUTILS_H

#include <filesystem>
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
    virtual ~ParserBase() = 0;
    virtual std::vector<char> at(int index) = 0;
    virtual int size() const = 0;
};

bool naturalCompare(const header &a, const header &b);

#endif // PARSEUTILS_H
