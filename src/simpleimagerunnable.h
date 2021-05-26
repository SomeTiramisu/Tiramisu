#ifndef SIMPLEIMAGERUNNABLE_H
#define SIMPLEIMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "parsers/parser.h"
#include "utils/utils.h"

class SimpleImageRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    SimpleImageRunnable(Parser &book, PageRequest req);
    ~SimpleImageRunnable() {};
    void run() override;
private:
    Parser &m_book;
    PageRequest m_req;

signals:
    void done(PageRequest req, QImage img);
};

#endif // SIMPLEIMAGERUNNABLE_H
