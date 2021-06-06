#ifndef SCALERUNNABLE_H
#define SCALERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include "../pagepreloader.h"
#include "../utils/utils.h"

class ScaleRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ScaleRunnable(PagePreloader* preloader, const PageRequest& req);
    ~ScaleRunnable() {};
    void run() override;
private:
    PagePreloader *m_preloader;
    PageRequest m_req;

signals:
    void imageReady(PageRequest req, QImage img);
};

#endif // SCALERUNNABLE_H
