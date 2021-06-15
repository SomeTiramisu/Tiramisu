#include "parseutils.h"

ParserBase::~ParserBase() {}

bool naturalCompare(const header &a, const header &b) {
    int r = strnatcasecmp(a.filename.c_str(), b.filename.c_str());
    return r < 0;
}
