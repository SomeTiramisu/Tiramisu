#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThreadPool>
#include <QPixmap>
#include "parsers/parser.h"
#include "utils/utils.h"

enum RequestStatus {
    NotRequested,
    Requested,
    Recieved
};

struct Pair {
    PageRequest req;
    RequestStatus status{RequestStatus::NotRequested};
    QImage img;

    bool matchRequest(const PageRequest& r) {
        return req == r;
    }

    bool matchStatus(const RequestStatus& s) {
        return status == s;
    }

    bool operator==(const Pair& a) const {
        return (req==a.req && status==a.status && img==a.img);
    }

    bool operator!=(const Pair& a) const {
        return not operator==(a);
    }
};

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
    QVector<Pair> pages;
    Parser book;
    PageRequest pendingReq;

public slots:
    void handleImage(PageRequest req, QImage img);
signals:
    void pageReady(QImage img);


};

#endif // PAGECONTROLLER_H
