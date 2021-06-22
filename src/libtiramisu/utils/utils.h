#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core.hpp>
#include <QtCore>
#include <iostream>
#include <functional>


using Path = std::string;
using ByteVect = std::vector<char>;
template <class T>
using Slot = std::function<void(T)> ;

struct PageRequest {
    int index{-1};
    int width{-1};
    int height{-1};
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
    bool operator==(const PagePair& other) const {
        return /*img==other.img &&*/ req==other.req;
    };
    bool operator!=(const PagePair& other) const {
        return !operator==(other);
    }
};
struct PngPair {
    ByteVect png;
    cv::Rect roi;
    bool operator==(const PngPair& other) const {
        return png==other.png && roi==other.roi;
    };
    bool operator!=(const PngPair& other) const {
        return !operator==(other);
    }
};

#endif // UTILS_H
