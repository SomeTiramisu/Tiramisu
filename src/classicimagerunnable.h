#ifndef CLASSICIMAGERUNNABLE_H
#define CLASSICIMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include "parsers/parser.h"
#include "utils/utils.h"

class ClassicImageRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ClassicImageRunnable(Parser &parser, PageRequest req);
    ~ClassicImageRunnable() {};
    void run() override;
private:
    Parser &m_parser;
    PageRequest m_req;

signals:
    void done(PageRequest req, QImage img);
};

#endif // CLASSICIMAGERUNNABLE_H
