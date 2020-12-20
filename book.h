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
    char* getSeek(int s);
    char* getAt(int index);
    unsigned int getLength();
    void setIndex(int n);
    void incIndex(int n);
    void decIndex(int n);
    int getSize();
private:
    archive *bookArchive;
    archive_entry *entry;
    char* buf = new char[0];
    unsigned int length;
    std::vector<header> headers;
    void openArchive(std::string filename);
    void loadBufAt(int n);
    int cindex;
    int size;
    QSettings settings;
};

bool naturalCompare(const header &a, const header &b);

#endif // BOOK_H
