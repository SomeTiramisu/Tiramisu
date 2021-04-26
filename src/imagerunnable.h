#ifndef IMAGERUNNABLE_H
#define IMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include "parsers/parser.h"
#include "utils/utils.h"

class ImageRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ImageRunnable(Parser &book, PageRequest req);
    ~ImageRunnable();
    void run() override;
private:
    Parser &m_book;
    PageRequest m_req;

signals:
    void done(PageRequest req, QImage img);
};

#endif // IMAGERUNNABLE_H
