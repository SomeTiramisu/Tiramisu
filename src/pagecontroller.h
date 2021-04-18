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
    void getAsyncPage(PageRequest req);
    QUrl getBookFilename();

private:
    void preloadPages(PageRequest req);
    void runPage(PageRequest req, int priority);
    void runLocalPage(PageRequest req);
    QThreadPool pool;
    QVector<PageResponseQ> pages;
    QVector<char> pagesStatus;
    Parser book;
    PageRequest pendingReq;

public slots:
    void handleImage(PageResponseCV resp);
signals:
    void pageReady(PageResponseQ resp);


};

#endif // PAGECONTROLLER_H
