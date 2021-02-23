#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUrl>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl bookFilename READ bookFilename WRITE setBookFilename NOTIFY bookFilenameChanged)
    Q_PROPERTY(QUrl bgFilename READ bgFilename WRITE setBgFilename NOTIFY bgFilenameChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(QString productName READ productName CONSTANT)

public:
    Backend();
    ~Backend();
    void setBookFilename(QUrl &f);
    void setBgFilename(QUrl &f);
    void setWidth(int &w);
    void setHeight(int &h);
    void setPageIndex(int &i);
    QUrl bookFilename();
    QUrl bgFilename();
    int width();
    int height();
    int pageIndex();
    int maxIndex();
    QString productName();
    bool init();

private:
    QUrl m_bookFilename;
    QUrl m_bgFilename;
    int m_width;
    int m_height;
    int m_pageIndex;
    int m_maxIndex;
    bool m_init;


signals:
    void bookFilenameChanged();
    void bgFilenameChanged();
    void widthChanged();
    void heightChanged();
    void pageIndexChanged();
};

#endif // BACKEND_H
