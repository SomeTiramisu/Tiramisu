#include "unarrparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>

UnarrParser::UnarrParser(const QUrl& fn)
    : m_filename(fn)
{
    if (isSupported(fn)) {
        ar_stream *s  = ar_open_file(fn.toLocalFile().toStdString().c_str());
        ar_archive *a = ar_open_rar_archive(s);;
        int i = 0;
        m_size = 0;
        while (ar_parse_entry(a)) {
            header h = {
                .filename = std::string(ar_entry_get_name(a)), //may need a *
                .index = i,
                .length = ar_entry_get_size(a)
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            i++;
        }
        //qWarning("book size: %i", size);
        ar_close_archive(a);
        ar_close(s);

        std::sort(m_headers.begin(), m_headers.end(), ParserUtils::naturalCompare);
    }
}

UnarrParser::UnarrParser(QByteArray* ramArchive)
    : m_isRam(true),
      m_ramArchive(ramArchive)
{
    if (isSupported()) {
        ar_stream *s = ar_open_memory(m_ramArchive->constData(), m_ramArchive->length());
        ar_archive *a = ar_open_rar_archive(s);
        int i = 0;
        m_size = 0;
        while (ar_parse_entry(a)) {
            header h = {
                .filename = std::string(ar_entry_get_name(a)), //may need a *
                .index = i,
                .length = ar_entry_get_size(a)
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            i++;
        }
        //qWarning("book size: %i", size);
        ar_close_archive(a);
        ar_close(s);

        std::sort(m_headers.begin(), m_headers.end(), ParserUtils::naturalCompare);
    }
}

QByteArray UnarrParser::at(int index) {
    int n = m_headers[index].index;
    ar_stream *s{nullptr};
    if (m_isRam) {
        s = ar_open_memory(m_ramArchive->constData(), m_ramArchive->length());
    } else {
        s  = ar_open_file(m_filename.toLocalFile().toStdString().c_str());
    }
    ar_archive *a = ar_open_rar_archive(s);;
    for (int i=0; i<=n; i++) {
        ar_parse_entry(a);
    }
    size_t length = m_headers[index].length;
    char* buf = new char[length];
    ar_entry_uncompress(a, buf, length);
    ar_close_archive(a);
    ar_close(s);
    QByteArray array(buf, length);
    //cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    delete[] buf;
    return array;
}

int UnarrParser::size() const {
    return m_size;
}

bool UnarrParser::isSupported(const QUrl& fn) {
    if (fn.isEmpty()) {
        return false;
    }
    ar_stream *s = ar_open_file(fn.toLocalFile().toStdString().c_str());
    if (!s) {
        return false;
    }
    ar_archive *a = ar_open_rar_archive(s);
    if (!a) {
        ar_close(s);
        return false;
    }
    ar_close_archive(a);
    ar_close(s);
    return true;

}

bool UnarrParser::isSupported() const {
    if (m_isRam) {
        ar_stream *s = ar_open_memory(m_ramArchive->constData(), m_ramArchive->length());
        if (!s) {
            return false;
        }
        ar_archive *a = ar_open_rar_archive(s);
        if (!a) {
            ar_close(s);
            return false;
        }
        ar_close_archive(a);
        ar_close(s);
        return true;
    } else {
        return isSupported(m_filename);
    }
}
