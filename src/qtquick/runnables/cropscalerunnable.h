#ifndef CROPSCALERUNNABLE_H
#define CROPSCALERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include "../pagepreloader.h"
#include "../utils/utils.h"

class CropScaleRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    CropScaleRunnable(PagePreloader* preloader, const PageRequest& req);
    ~CropScaleRunnable() {};
    void run() override;
private:
    PagePreloader *m_preloader;
    PageRequest m_req;

signals:
    void imageReady(PageRequest req, QImage img);
};

#endif // CROPSCALERUNNABLE_H
