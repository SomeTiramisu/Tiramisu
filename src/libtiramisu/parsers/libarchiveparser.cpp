#include "libarchiveparser.h"

#include <opencv2/imgcodecs.hpp>

LibarchiveParser::LibarchiveParser(const Path& fn)
    : m_filename(fn)
{
    if (isSupported(fn)) {
        archive* a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_zip(a);
        archive_read_open_filename(a, m_filename.c_str(),  10240);
        int i = 0;
        m_size = 0;
        archive_entry *entry;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            header h = {
                std::string(archive_entry_pathname(entry)),
                i,
                static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            archive_read_data_skip(a);
            i++;
        }
        archive_read_close(a);
        archive_read_free(a);
        std::sort(m_headers.begin(), m_headers.end(), &naturalCompare);
    }
}

LibarchiveParser::LibarchiveParser(const ByteVect& ramArchive)
    : m_isRam(true),
      m_ramArchive(ramArchive)
{
    if (isSupported(m_ramArchive)) {
        archive* a = archive_read_new();
        archive_read_support_filter_all(a);
        archive_read_support_format_zip(a);
        archive_read_open_memory(a, m_ramArchive.data(), m_ramArchive.size());
        int i = 0;
        m_size = 0;
        archive_entry *entry;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            header h = {
                std::string(archive_entry_pathname(entry)),
                i,
                static_cast<size_t>(archive_entry_size(entry))
            };
            if (h.filename.rfind(".jpg") != std::string::npos || h.filename.rfind(".png") != std::string::npos) {
                m_headers.push_back(h);
                m_size++;
            }
            archive_read_data_skip(a);
            i++;
        }
        archive_read_close(a);
        archive_read_free(a);
        std::sort(m_headers.begin(), m_headers.end(), &naturalCompare);
    }
}

ByteVect LibarchiveParser::at(int index) const {
    int n = m_headers[index].index;
    archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    if (m_isRam) {
        archive_read_open_memory(a, m_ramArchive.data(), m_ramArchive.size());
    } else {
        archive_read_open_filename(a, m_filename.c_str(),  10240);
    }
    archive_entry *entry{nullptr};
    for (int i=0; i<=n; i++) {
        archive_read_next_header(a, &entry);
    }
    size_t length = m_headers[index].length;
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(length);
    archive_read_data(a, buf.get(), length);
    archive_read_close(a);
    archive_read_free(a);
    ByteVect array;
    array.assign(buf.get(), buf.get()+length);
    //cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    return array;
}

int LibarchiveParser::size() const {
    return m_size;
}

bool LibarchiveParser::isSupported(const Path& fn) {
    if (fn.empty()) {
        return false;
    }
    archive *a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    int r = archive_read_open_filename(a, fn.c_str(), 10240);
    if (r != ARCHIVE_OK) {
        return false;
    }
    archive_read_close(a);
    archive_read_free(a);
    return true;

}

bool LibarchiveParser::isSupported(const ByteVect& ramArchive) {
    if (ramArchive.empty()) {
        return false;
    }
    archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_zip(a);
    int r = archive_read_open_memory(a, ramArchive.data(), ramArchive.size());
    if (r != ARCHIVE_OK) {
        return false;
    }
    archive_read_close(a);
    archive_read_free(a);
    return true;
}
