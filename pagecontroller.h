#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include "backend.h"
#include "pageworker.h"

class PageController : public QObject
{
    Q_OBJECT
public:
    PageController(Backend* b, QObject *parent = nullptr);
    ~PageController();
    QImage getPage();
    void getAsyncPage();
    void initPage(int index);


private:
    void preloadPages(int index);
    void cleanPages(int maxIndex);
    Backend* backend;
    ImageWorker *worker;
    QThread workerThread;
    QVector<QImage> pages;
    QVector<char> pagesStatus;
    int lastIndex;

public slots:
    void handleImage(Page page);
    void changeBookFilename();
signals:
    void addImage(QUrl book_filename, int index, int width, int height);
    void addPage(QImage image);


};

#endif // PAGECONTROLLER_H
