#ifndef TIRAMISU_H
#define TIRAMISU_H

#include <QObject>
#include <QUrl>
#include "utils/utils.h"
#include "pagepreloader.h"
#include "pagescheduler.h"

class Tiramisu: public QObject
{
    Q_OBJECT
public:
    Tiramisu(QUrl filename = QUrl());
    ~Tiramisu();
    QImage get(PageRequest req);
    int bookSize() const {return m_bookSize;}
    int preloaderProgress() const { return m_preloader->progress();}

private:
    void setFilename(const QUrl& filename);
    QUrl m_filename;
    int m_bookSize{0};
    PageScheduler* m_scheduler{nullptr};
    PagePreloader* m_preloader{nullptr};
    PageRequest m_req;
};

#endif // TIRAMISU_H
