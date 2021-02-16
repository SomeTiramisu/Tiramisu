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
    QPixmap* getPage();
    void initPage(int index);


private:
    void preloadPages(int index);
    Backend* backend;
    ImageWorker *worker;
    QThread workerThread;
    QPixmap* *pages;
    char *pagesStatus;
    int lastIndex;

public slots:
    void handleImage(QPixmap* img, int index);
signals:
    void addImage(QString book_filename, QString bg_filename, int index, int width, int height);

};

#endif // PAGECONTROLLER_H
