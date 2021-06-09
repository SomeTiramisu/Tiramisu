#ifndef PAGEPRELOADER_H
#define PAGEPRELOADER_H

#include <QByteArray>
#include <QUrl>
#include <QVector>
#include <QObject>
#include <QThreadPool>
#include <QByteArray>
#include <opencv2/core.hpp>
#include "parsers/parser.h"

struct PngPair {
    QByteArray png;
    cv::Rect roi;
};

class PagePreloader: public QObject
{
    Q_OBJECT
public:
    PagePreloader(QUrl filename = QUrl(), QObject* parent = nullptr);
    ~PagePreloader();
    PngPair at(int index);
    void runCrop(int index);
    void runLocalCrop(int index);
    int size() const;
    QUrl filename() const;
    int progress();

private:
    Parser *m_parser{nullptr};
    QVector<PngPair> m_pages;
    QUrl m_filename;
    QThreadPool m_pool;
    bool isReady{false};
    int m_progress{0};

signals:
    void progressChanged();

public slots:
    void handleRoi(int index, QByteArray png, cv::Rect roi);
};

#endif // PAGEPRELOADER_H
