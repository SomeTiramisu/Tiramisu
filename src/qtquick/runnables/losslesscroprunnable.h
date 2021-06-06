#ifndef LOSSLESSCROPRUNNABLE_H
#define LOSSLESSCROPRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "../parsers/parser.h"

class LosslessCropRunnable: public QObject, public QRunnable
{
    Q_OBJECT
public:
    LosslessCropRunnable(Parser* parser, int index);
    ~LosslessCropRunnable() {};
    void run() override;

private:
    int m_index;
    Parser* m_parser;

signals:
    void pngReady(int index, QByteArray array);
};

#endif // LOSSLESSCROPRUNNABLE_H
