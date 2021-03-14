#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include "pageworker.h"
#include "helper.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(QUrl book_filename, QObject *parent = nullptr);
    ~PageController();
    QImage getPage(PageRequest req);
    void getAsyncPage(PageRequest req);
    void initPage(PageRequest req);
    QUrl getBookFilename();


private:
    void preloadPages(PageRequest req);
    void cleanPages(int maxIndex);
    ImageWorker *worker;
    QThread workerThread;
    QVector<QImage> pages;
    QVector<char> pagesStatus;

    int lastIndex;
    QUrl book_filename;
    int book_size;


public slots:
    void handleImage(Page page);
signals:
    void addImage(int index, int width, int height);
    void addPage(QImage image);


};

#endif // PAGECONTROLLER_H
