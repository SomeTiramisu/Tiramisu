#include "libarchiveparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>

LibarchiveParser::LibarchiveParser(QUrl fn, bool toram)
    : filename(fn),
      isRam(toram)
{
    if (isSupported(fn)) {
        if (isRam) {
            initRamArchive();
            openRamArchive();
        } else {
            openArchive();
        }
        int i = 0;
        size = 0;
        while (archive_read_next_header(bookArchive, &entry) == ARCHIVE_OK) {
            header h = {
                .filename = std::string(archive_entry_pathname(entry)),
                .index = i,
                .length = static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                headers.push_back(h);
                size++;
            }
            archive_read_data_skip(bookArchive);
            i++;
        }
    //qWarning("book size: %i", size);
    archive_read_free(bookArchive);
    /*
    for (std::vector<header>::iterator it=headers.begin(); it !=headers.end(); ++it) {
        qWarning("%s", it->filename.c_str());
    }
    */
    std::sort(headers.begin(), headers.end(), ParserUtils::naturalCompare);
    }
}

void LibarchiveParser::openArchive() {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    archive_read_open_filename(bookArchive, filename.toLocalFile().toStdString().c_str(),  10240);
}

void LibarchiveParser::openRamArchive() {
    bookArchive = archive_read_new();
    archive_read_support_filter_all(bookArchive);
    archive_read_support_format_zip(bookArchive);
    //archive_read_support_format_rar(bookArchive); buggy
    archive_read_open_memory(bookArchive, ram_archive.data(), ram_archive.length()); //may be incorrect
}

void LibarchiveParser::initRamArchive() {
    QFile file(filename.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ram_archive = file.readAll();
    file.close();
}

cv::Mat LibarchiveParser::getAt(int index) {
    int n = headers[index].index;
    if (isRam) {
        openRamArchive();
    } else {
        openArchive();
    }
    for (int i=0; i<=n; i++) {
        archive_read_next_header(bookArchive, &entry);
    }
    size_t length = headers[index].length;
    //delete[] buf; deleted after Image creation
    char* buf = new char[length]; //TODO: use voud buffer
    archive_read_data(bookArchive, buf, length);
    archive_read_free(bookArchive);
    cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    delete[] buf;
    return img;
}

int LibarchiveParser::getSize() {
    return size;
}

QUrl LibarchiveParser::getFilename() {
    return filename;
}

bool LibarchiveParser::isSupported(QUrl fn) {
    if (fn.isEmpty()) {
        return false;
    }
    struct archive *a;
    int r;
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    r = archive_read_open_filename(a, fn.toLocalFile().toStdString().c_str(), 10240);
    if (r != ARCHIVE_OK) {
        return false;
    }
    archive_read_free(a);
    return  true;

}
