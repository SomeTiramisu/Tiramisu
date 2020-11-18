#include "book.h"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
};

Book::Book()
{
    archive = archive_read_new();
    archive_read_support_filter_all(archive);
    archive_read_support_format_zip(archive);
    archive_read_support_format_rar(archive);
    archive_read_open_filename(archive, "/storage/emulated/0/b.cbr", 10240);


    buf = new char[length];
    archive_read_next_header(archive, &entry);
    length = archive_entry_size(entry);
    archive_read_data(archive, buf, length);


}

Book::~Book()
{
    delete[] buf;
    archive_read_free(archive);
}

char* Book::getCurrent()
{
    return buf;
}

char* Book::getNext()
{
    archive_read_next_header(archive, &entry);
    length = archive_entry_size(entry);
    archive_read_data(archive, buf, length);
    n++;
    return buf;
}

char* Book::getPrevious()
{

    return buf;
}


unsigned int Book::getLength()
{
    return length;
}
