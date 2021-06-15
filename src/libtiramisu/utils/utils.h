#ifndef UTILS_H
#define UTILS_H

#include <filesystem>
#include <opencv2/core.hpp>


using Path = std::filesystem::path;
using ByteVect = std::vector<char>;

class PageRequest {
public:
    PageRequest() = default;
    PageRequest(int width, int height, int index, Path filename);
    PageRequest addIndex(int i) const;
    ~PageRequest() = default;
    int width() const {return m_width;};
    int height() const {return m_height;};
    int index() const {return m_index;};
    Path filename() const {return m_filename;};
    bool valid() const {return m_valid;};
    void setValid(bool valid);
    bool isLike(const PageRequest& a) const;
    bool operator==(const PageRequest& a) const;
    bool operator!=(const PageRequest& a) const;
private:
    int m_width{-1};
    int m_height{-1};
    int m_index{-1};
    Path m_filename;
    bool m_valid{false};
};

struct PagePair {
    cv::Mat img;
    PageRequest req;
};
struct PngPair {
    ByteVect png;
    cv::Rect roi;
};

#endif // UTILS_H
