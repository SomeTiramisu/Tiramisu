#ifndef UNARRPARSER_H
#define UNARRPARSER_H

#include "parseutils.h"

extern "C" {
#include <unarr.h>
}

class UnarrParser: public ParserBase {

public:
    UnarrParser(const QUrl& fn);
    UnarrParser(QByteArray* ramArchive);
    ~UnarrParser() override {};
    QByteArray at(int index) override;
    int size() const override;
    static bool isSupported(const QUrl& fn);
    bool isSupported() const override;

private:
    std::vector<header> m_headers;
    int m_size{0};
    QUrl m_filename;
    bool m_isRam{false};
    QByteArray* m_ramArchive;
};

#endif // UNARRPARSER_H
