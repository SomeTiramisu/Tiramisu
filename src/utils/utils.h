#ifndef UTILS_H
#define UTILS_H

#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QImage>

class PageAnswer: public QObject {
    Q_OBJECT
public:
    void answer(QImage img) {
        emit s_answer(img);
    }
signals:
    void s_answer(QImage img);
};

class PageRequest {
public:
    PageRequest(int width, int height, int index, QUrl book_filename, QString controller_id, int controller_preload, QString runnable_type);
    static PageRequest fromId(const QString& id);
    PageRequest() {};
    PageRequest addIndex(int i) const;
    ~PageRequest();
    int width() const {return m_width;};
    int height() const {return m_height;};
    int index() const {return m_index;};
    QUrl book_filename() const {return m_book_filename;};
    QString controller_id() const {return m_controller_id;};
    int controller_preload() const {return m_controller_preload;};
    QString runnableType() const {return m_runnableType;}
    bool isLike(const PageRequest& a) const;
    bool isInRange(const PageRequest& a, int d) const;
    bool operator==(const PageRequest& a) const;
    bool operator!=(const PageRequest& a) const;
private:
    int m_width{-1};
    int m_height{-1};
    int m_index{-1};
    QUrl m_book_filename;
    QString m_controller_id;
    int m_controller_preload;
    QString m_runnableType{"simple"};

};

class Utils {
public:
    static void cleanupPageImage(void* info) { //info is pointing to image data
        uchar* data = static_cast<uchar*>(info);
        delete[] data;
        //qWarning("data deleted");
    }
};

#endif // UTILS_H
