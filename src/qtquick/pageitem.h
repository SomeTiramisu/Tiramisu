#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QQuickPaintedItem>

class PageItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString schedulerId READ schedulerId WRITE setSchedulerId NOTIFY schedulerIdChanged)
    Q_PROPERTY(int schedulerPreload READ schedulerPreload WRITE setSchedulerPreload NOTIFY schedulerPreloadChanged)
    Q_PROPERTY(QString runnableType READ runnableType WRITE setRunnableType NOTIFY runnableTypeChanged)

public:
    PageItem();
    ~PageItem() {};
    QUrl filename() {return m_filename;};
    int index() {return m_index;};
    QString schedulerId() {return m_schedulerId;};
    int schedulerPreload() {return m_schedulerPreload;};
    QString runnableType() {return m_runnableType;};
    void setFilename(QUrl& filename) {};
    void setIndex(int index) {};
    void setSchedulerId(QString& schedulerId) {};
    void setSchedulerPreload(int schedulerPreload) {};
    void setRunnableType(QString& runnableType) {};
    void paint(QPainter *painter) override;

private:
    QUrl m_filename;
    int m_index;
    QString m_schedulerId;
    int m_schedulerPreload;
    QString m_runnableType;

signals:
    void filenameChanged();
    void indexChanged();
    void schedulerIdChanged();
    void schedulerPreloadChanged();
    void runnableTypeChanged();

};

#endif // PAGEITEM_H
