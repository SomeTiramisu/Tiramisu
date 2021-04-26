#include "libarchiveparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>

LibarchiveParser::LibarchiveParser(const QUrl& fn)
    : m_filename(fn)
{
    if (isSupported(fn)) {
        archive* a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_zip(a);
        archive_read_open_filename(a, m_filename.toLocalFile().toStdString().c_str(),  10240);
        int i = 0;
        m_size = 0;
        archive_entry *entry;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            header h = {
                .filename = std::string(archive_entry_pathname(entry)),
                .index = i,
                .length = static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            archive_read_data_skip(a);
            i++;
        }
        archive_read_free(a);
        std::sort(m_headers.begin(), m_headers.end(), ParserUtils::naturalCompare);
    }
}

LibarchiveParser::LibarchiveParser(QByteArray* ramArchive)
    : m_isRam(true),
      m_ramArchive(ramArchive)
{
    if (isSupported()) {
        archive* a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_zip(a);
        archive_read_open_memory(a, m_ramArchive->constData(), m_ramArchive->length());
        int i = 0;
        m_size = 0;
        archive_entry *entry;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            header h = {
                .filename = std::string(archive_entry_pathname(entry)),
                .index = i,
                .length = static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            archive_read_data_skip(a);
            i++;
        }
        archive_read_free(a);
        std::sort(m_headers.begin(), m_headers.end(), ParserUtils::naturalCompare);
    }
}

cv::Mat LibarchiveParser::getAt(int index) {
    int n = m_headers[index].index;
    archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    if (m_isRam) {
        archive_read_open_memory(a, m_ramArchive->constData(), m_ramArchive->length());
    } else {
        archive_read_open_filename(a, m_filename.toLocalFile().toStdString().c_str(),  10240);
    }
    archive_entry *entry{nullptr};
    for (int i=0; i<=n; i++) {
        archive_read_next_header(a, &entry);
    }
    size_t length = m_headers[index].length;
    char* buf = new char[length]; //TODO: use voud buffer
    archive_read_data(a, buf, length);
    archive_read_free(a);
    cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    delete[] buf;
    return img;
}

int LibarchiveParser::getSize() const {
    return m_size;
}

bool LibarchiveParser::isSupported(const QUrl& fn) {
    if (fn.isEmpty()) {
        return false;
    }
    archive *a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    int r = archive_read_open_filename(a, fn.toLocalFile().toStdString().c_str(), 10240);
    if (r != ARCHIVE_OK) {
        return false;
    }
    archive_read_free(a);
    return  true;

}

bool LibarchiveParser::isSupported() const {
    if (m_isRam) {
        archive* a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_zip(a);
        int r = archive_read_open_memory(a, m_ramArchive->constData(), m_ramArchive->length());
        if (r != ARCHIVE_OK) {
            return false;
        }
        archive_read_free(a);
        return  true;
    } else {
        return isSupported(m_filename);
    }
}
