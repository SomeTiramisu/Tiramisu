#ifndef UTILS_H
#define UTILS_H

#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QImage>
#include <opencv2/core.hpp>

class PageRequest {
public:
    PageRequest(int width, int height, int index, QUrl filename);
    PageRequest() {};
    PageRequest addIndex(int i) const;
    ~PageRequest();
    int width() const {return m_width;};
    int height() const {return m_height;};
    int index() const {return m_index;};
    QUrl filename() const {return m_filename;};
    bool isLike(const PageRequest& a) const;
    bool isInRange(const PageRequest& a, int d) const;
    bool operator==(const PageRequest& a) const;
    bool operator!=(const PageRequest& a) const;
private:
    int m_width{-1};
    int m_height{-1};
    int m_index{-1};
    QUrl m_filename;
};


class Utils {
public:
    static void cleanupPageImage(void* info) { //info is pointing to image data
        uchar* data = reinterpret_cast<uchar*>(info);
        delete[] data;
        //qWarning("data deleted");
    }
};

Q_DECLARE_METATYPE(PageRequest);
Q_DECLARE_METATYPE(cv::Rect);

#endif // UTILS_H
