#ifndef UTILS_H
#define UTILS_H

#include <QUrl>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <opencv2/core.hpp>

struct PageRequest {
    int width;
    int height;
    int index;
    QUrl book_filename;
};

struct PageResponseQ: public PageRequest {
    QImage img;
};

struct PageResponseCV: public PageRequest {
    cv::Mat img;
};

class Utils {
public:
    static PageRequest decodeId(QString id) {
        //qWarning("id: %s", id.toStdString().c_str());
        QJsonObject jido = QJsonDocument::fromJson(QUrl::fromPercentEncoding(id.toUtf8()).toUtf8()).object(); //boncourage
        PageRequest ret = PageRequest {
                .width = jido.value("width").toInt(),
                .height = jido.value("height").toInt(),
                .index = jido.value("index").toInt(),
                .book_filename = jido.value("book_filename").toString()
    };
        qWarning("decoded: %i, %i, %i, %s", ret.width, ret.height, ret.index, ret.book_filename.toString().toStdString().c_str());
        return ret;
    }
};

#endif // UTILS_H
