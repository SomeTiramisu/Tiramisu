#ifndef CROPRUNNABLE_H
#define CROPRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "parsers/parser.h"

class CropRunnable: public QObject, public QRunnable
{
    Q_OBJECT
public:
    CropRunnable(Parser* parser, int index);
    ~CropRunnable() {};
    void run() override;

private:
    int m_index;
    Parser* m_parser;

signals:
    void pngReady(int index, QByteArray array);
};

#endif // CROPRUNNABLE_H
