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
    std::vector<char> png;
    cv::Rect roi;
};

class PagePreloader: public QObject
{
    Q_OBJECT
public:
    PagePreloader(std::filesystem::path& filename, QObject* parent = nullptr);
    ~PagePreloader();
    PngPair at(int index);
    void runCrop(int index);
    void runLocalCrop(int index);
    int size() const;
    std::filesystem::path filename() const;
    int progress();

private:
    std::unique_ptr<Parser> m_parser;
    QVector<PngPair> m_pages;
    std::filesystem::path m_filename;
    QThreadPool m_pool;
    bool isReady{false};
    int m_progress{0};

signals:
    void progressChanged();

public slots:
    void handleRoi(int index, std::vector<char> png, cv::Rect roi);
};

#endif // PAGEPRELOADER_H
