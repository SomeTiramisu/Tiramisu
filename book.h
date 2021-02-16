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
    long long length;
};

class Book
{
public:
    Book(std::string fn);
    ~Book();
    char* getAt(int index);
    long long getLength(int index);
    int getSize();
    std::string getFilename();

private:
    archive *bookArchive;
    archive_entry *entry;
    std::vector<header> headers;
    void openArchive(std::string filename);
    int size;
    std::string filename;

};

bool naturalCompare(const header &a, const header &b);

#endif // BOOK_H
