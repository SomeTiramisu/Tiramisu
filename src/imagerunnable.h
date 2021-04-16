#ifndef IMAGERUNNABLE_H
#define IMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
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
    Parser &book;
    PageRequest req;

signals:
    void done(PageResponseCV page);
};

#endif // IMAGERUNNABLE_H
