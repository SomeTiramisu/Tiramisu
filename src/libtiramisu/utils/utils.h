#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core.hpp>
#include <QtCore>
#include <iostream>
#include <functional>
#include <optional>


using Path = std::string;
//template <class T>
//using Option = std::optional<T>;
using ByteVect = std::vector<char>;
template <class T>
using Slot = std::function<void(const T&)> ;

//untested
template <class R, class T>
Slot<T> makeSlot(R* t, void(*f)(const T&)) {
    return [t, f](const T& r){t->f(r);};
}
template <class R, class T>
Slot<T> makeDefaultSlot(R* t, void(*f)(const T&)) {
    return [t, f](const T& r){t->f(r);};
}

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
/*
template <class T>
class Option { //because ndk r21 do not know <optional> for some reasons
public:
    bool has_value() const {
        return v!=nullptr;
    }
    T& value() const {
        if(!v) {
            throw std::terminate;
        }
        return *v;
    }
    template<class U=T>
    Option& operator=(U&& value) {
        if(*v != value) {
            v = std::make_unique<T>(value);
        }
        return *this;
    }
    void reset() {
        v.reset();
    }
private:
    std::unique_ptr<T> v;
};
*/
#endif // UTILS_H
