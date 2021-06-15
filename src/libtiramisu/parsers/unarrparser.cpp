#include "unarrparser.h"

#include <QtCore>
#include <opencv2/imgcodecs.hpp>
#include <memory>

UnarrParser::UnarrParser(const std::filesystem::path& fn)
    : m_filename(fn)
{
    if (isSupported(fn)) {
        ar_stream *s  = ar_open_file(fn.native().c_str());
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

        std::sort(m_headers.begin(), m_headers.end(), &naturalCompare);
    }
}

UnarrParser::UnarrParser(std::vector<char>& ramArchive)
    : m_isRam(true),
      m_ramArchive(ramArchive)
{
    if (isSupported(m_ramArchive)) {
        ar_stream *s = ar_open_memory(m_ramArchive.data(), m_ramArchive.size());
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

        std::sort(m_headers.begin(), m_headers.end(), &naturalCompare);
    }
}

std::vector<char> UnarrParser::at(int index) {
    int n = m_headers[index].index;
    ar_stream *s{nullptr};
    if (m_isRam) {
        s = ar_open_memory(m_ramArchive.data(), m_ramArchive.size());
    } else {
        s  = ar_open_file(m_filename.native().c_str());
    }
    ar_archive *a = ar_open_rar_archive(s);;
    for (int i=0; i<=n; i++) {
        ar_parse_entry(a);
    }
    size_t length = m_headers[index].length;
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(length);
    ar_entry_uncompress(a, buf.get(), length);
    ar_close_archive(a);
    ar_close(s);
    std::vector<char> array;
    array.assign(buf.get(), buf.get()+length);
    //cv::Mat img = imdecode(cv::Mat(1, length, CV_8UC1, buf), cv::IMREAD_COLOR);
    return array;
}

int UnarrParser::size() const {
    return m_size;
}

bool UnarrParser::isSupported(const std::filesystem::path& fn) {
    if (fn.empty()) {
        return false;
    }
    ar_stream *s = ar_open_file(fn.native().c_str());
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

bool UnarrParser::isSupported(const std::vector<char>& ramArchive) {
    if (ramArchive.empty()) {
        return false;
    }
    ar_stream *s = ar_open_memory(ramArchive.data(), ramArchive.size());
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
