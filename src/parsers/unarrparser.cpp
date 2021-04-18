#include "unarrparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>

UnarrParser::UnarrParser(QUrl fn, bool toram)
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
        while (ar_parse_entry(bookArchive)) {
            header h = {
                .filename = std::string(ar_entry_get_name(bookArchive)), //may need a *
                .index = i,
                .length = ar_entry_get_size(bookArchive)
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                headers.push_back(h);
                size++;
            }
            i++;
        }
    qWarning("book size: %i", size);
    ar_close_archive(bookArchive);
    ar_close(bookStream);

    std::sort(headers.begin(), headers.end(), ParserUtils::naturalCompare);
    }
}

void UnarrParser::openArchive() {
    bookStream = ar_open_file(filename.toLocalFile().toStdString().c_str());
    bookArchive = ar_open_rar_archive(bookStream);
}

void UnarrParser::openRamArchive() {
    bookStream = ar_open_memory(ram_archive.data(), ram_archive.length());
    bookArchive = ar_open_rar_archive(bookStream);
}

void UnarrParser::initRamArchive() {
    QFile file(filename.toLocalFile());
    file.open(QIODevice::ReadOnly);
    ram_archive = file.readAll();
    file.close();
}

PageResponseCV UnarrParser::getAt(int index) {
    int n = headers[index].index;
    if (isRam) {
        openRamArchive();
    } else {
        openArchive();
    }
    for (int i=0; i<=n; i++) {
        ar_parse_entry(bookArchive);
    }
    size_t length = headers[index].length;
    //delete[] buf; deleted after Image creation
    char* buf = new char[length];
    ar_entry_uncompress(bookArchive, buf, length);
    ar_close_archive(bookArchive);
    ar_close(bookStream);
    cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    PageResponseCV p = {{img.cols, img.rows, index, filename}, img};
    delete[] buf;
    return p;
}

int UnarrParser::getSize() {
    return size;
}

QUrl UnarrParser::getFilename() {
    return filename;
}

bool UnarrParser::isSupported(QUrl fn) {
    if (fn.isEmpty()) {
        return false;
    }
    ar_stream *s  = nullptr;
    ar_archive *a = nullptr;
    s = ar_open_file(fn.toLocalFile().toStdString().c_str());
    if (!s) {
        return false;
    }
    a = ar_open_rar_archive(s);
    if (!a) {
        ar_close(s);
        return false;
    }
    ar_close_archive(a);
    ar_close(s);
    return true;

}
