#ifndef CLASSICIMAGERUNNABLE_H
#define CLASSICIMAGERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include "pagepreloader.h"
#include "utils/utils.h"

class ClassicImageRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ClassicImageRunnable(PagePreloader* preloader, const PageRequest& req);
    ~ClassicImageRunnable() {};
    void run() override;
private:
    PagePreloader *m_preloader;
    PageRequest m_req;

signals:
    void imageReady(PageRequest req, QImage img);
};

#endif // CLASSICIMAGERUNNABLE_H
