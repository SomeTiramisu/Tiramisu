#ifndef POPPLERPARSER_H
#define POPPLERPARSER_H

#include "parseutils.h"
#include <poppler/cpp/poppler-document.h>

class PopplerBook {

public:
    PopplerBook(QUrl fn);
    ~PopplerBook() {};
    Page getAt(int index);
    int getSize();
    static bool isSupported(QUrl fn);
    QUrl getFilename();

private:
    poppler::document *bookDoc;
    void openArchive();
    void initArchive(QUrl fn);
    int size;
    QUrl filename;
    QByteArray ram_archive;
};

#endif // POPPLERPARSER_H
