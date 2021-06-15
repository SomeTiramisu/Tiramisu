#ifndef CROPDETECTRUNNABLE_H
#define CROPDETECTRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <opencv2/core.hpp>
#include "../parsers/parser.h"

class CropDetectRunnable: public QObject, public QRunnable
{
    Q_OBJECT
public:
    CropDetectRunnable(Parser* parser, int index);
    ~CropDetectRunnable() {};
    void run() override;

private:
    int m_index;
    Parser* m_parser;

signals:
    void roiReady(int index, std::vector<char> png, cv::Rect roi);
};

#endif // CROPDETECTRUNNABLE_H
