#ifndef BOOK_H
#define BOOK_H
extern "C" {
#include <archive.h>
#include <archive_entry.h>
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
    struct archive *archive;
    struct archive_entry *entry;
    char* buf;
    unsigned int length;
};

#endif // BOOK_H
