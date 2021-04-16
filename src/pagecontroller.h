#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThreadPool>
#include <QPixmap>
#include "parsers/parser.h"
#include "utils/utils.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(QUrl book_filename, QObject *parent = nullptr);
    ~PageController();
    QImage getPage(PageRequest req);
    void getAsyncPage(PageRequest req);
    QUrl getBookFilename();


private:
    void preloadPages(PageRequest req);
    void cleanPages(int maxIndex);
    void runPage(PageRequest req, int priority);
    void runLocalPage(PageRequest req);
    QThreadPool pool;
    QVector<QImage> pages;
    QVector<char> pagesStatus;
    Parser book;

    int lastIndex;
    int pendingIndex;
    QUrl book_filename;
    int book_size;

public slots:
    void handleImage(PageResponseCV page);
signals:
    void pageReady(QImage image);


};

#endif // PAGECONTROLLER_H
