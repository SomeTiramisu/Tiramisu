#ifndef PAGESCHEDULER_H
#define PAGESCHEDULER_H

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

enum RequetPriority {
    Max = 1,
    Req = 0
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

class PageScheduler : public QObject
{
    Q_OBJECT
public:
    PageScheduler(const QUrl& filename = QUrl(), int imgPrld = -1, QObject *parent = nullptr);
    ~PageScheduler();
    void getAsyncPage(PageRequest req, PageAnswer* ans);
    QUrl getBookFilename();

private:
    void preloadPages(PageRequest req);
    void clearPages(PageRequest req);
    void runPage(PageRequest req, RequetPriority priority);
    void runLocalPage(PageRequest req);
    QThreadPool m_pool;
    QHash<PageRequest, Pair> m_pages;
    Parser m_parser;
    QHash<PageRequest, PageAnswer*> m_pendingReqs;
    const int m_imagePreload;
    QMutex m_lock;

public slots:
    void handleImage(PageRequest req, QImage img);
//signals:
    //void pageReady(PageRequest req, QImage img);


};

static inline uint qHash(const PageRequest& req, uint seed) {
    return qHash(req.width() << 20 | req.height()<<10 | req.index(), seed) ^ qHash(req.filename(), seed);
}

#endif // PAGESCHEDULER_H
