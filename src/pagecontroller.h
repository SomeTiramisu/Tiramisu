#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThreadPool>
#include <QHash>
#include <QSet>
#include <QImage>
#include "parsers/parser.h"
#include "utils/utils.h"

enum RequestStatus {
    NotRequested,
    Requested,
    Recieved,
    Undefined
};

struct Pair {
    RequestStatus status{RequestStatus::Undefined};
    QImage img;

    bool matchStatus(const RequestStatus& s) const {
        return status == s;
    }

    bool operator==(const Pair& a) const {
        return (status==a.status && img==a.img);
    }

    bool operator!=(const Pair& a) const {
        return not operator==(a);
    }
};

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(const QUrl& book_filename, bool toram = false, int imgprld = -1, QObject *parent = nullptr);
    ~PageController();
    void getAsyncPage(PageRequest req, PageAnswer* ans);
    QUrl getBookFilename();

private:
    void preloadPages(PageRequest req);
    void clearPages(PageRequest req);
    void runPage(PageRequest req, int priority);
    void runLocalPage(PageRequest req);
    QThreadPool pool;
    QHash<PageRequest, Pair> pages;
    Parser book;
    QHash<PageRequest, PageAnswer*> pendingReqs;
    const int imagePreload;
    QMutex lock;

public slots:
    void handleImage(PageRequest req, QImage img);
//signals:
    //void pageReady(PageRequest req, QImage img);


};

static inline uint qHash(const PageRequest& req, uint seed) {
    return qHash(req.width() << 20 | req.height()<<10 | req.index(), seed) ^ qHash(req.book_filename(), seed);
}

#endif // PAGECONTROLLER_H
