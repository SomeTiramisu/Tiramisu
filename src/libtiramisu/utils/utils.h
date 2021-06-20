#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core.hpp>
#include <QtCore>
#include <iostream>


using Path = std::string;
using ByteVect = std::vector<char>;

struct PageRequest {
    int index;
    int width;
    int height;
    Path filename;
    bool operator==(const PageRequest& other) const {
        return index==other.index
                && width==other.width
                && height==other.height
                && filename==other.filename;
    };
    bool operator!=(const PageRequest& other) const {
        return !operator==(other);
    };
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
