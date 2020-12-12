#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <QSettings>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
};

struct header {
    std::string filename;
    int index;
};

class Book
{
public:
    Book();
    ~Book();
    char* getCurrent();
    char* getNext();
    char* getPrevious();
    unsigned int getLength();
private:
    archive *bookArchive;
    archive_entry *entry;
    char* buf = new char[0];
    unsigned int length;
    std::vector<header> headers;
    void openArchive(std::string filename);
    void loadBufAt(int n);
    int cindex;
    QSettings settings;
};

bool naturalCompare(const header &a, const header &b);

#endif // BOOK_H
