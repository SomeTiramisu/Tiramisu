#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include "backend.h"
#include "pageworker.h"

struct PageRequest {
    int width;
    int height;
    int index;
    QUrl book_filename;
};

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(Backend* b, QObject *parent = nullptr);
    ~PageController();
    QImage getPage(QString id);
    void getAsyncPage(QString id);
    void initPage(PageRequest req);


private:
    void preloadPages(PageRequest req);
    void cleanPages(int maxIndex);
    void changeBookFilename(QUrl book_filename);
    PageRequest decodeId(QString id);
    Backend* backend;
    ImageWorker *worker;
    QThread workerThread;
    QVector<QImage> pages;
    QVector<char> pagesStatus;

    int lastIndex;
    QUrl last_book_filename;


public slots:
    void handleImage(Page page);
signals:
    void addImage(QUrl book_filename, int index, int width, int height);
    void addPage(QImage image);


};

#endif // PAGECONTROLLER_H
