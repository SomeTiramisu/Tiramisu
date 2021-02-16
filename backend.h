#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString bookFilename READ bookFilename WRITE setBookFilename NOTIFY bookFilenameChanged)
    Q_PROPERTY(QString bgFilename READ bgFilename WRITE setBgFilename NOTIFY bgFilenameChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(QString productName READ productName CONSTANT)

public:
    Backend();
    ~Backend();
    void setBookFilename(QString &f);
    void setBgFilename(QString &f);
    void setWidth(int &w);
    void setHeight(int &h);
    void setPageIndex(int &i);
    QString bookFilename();
    QString bgFilename();
    int width();
    int height();
    int pageIndex();
    int previousIndex();
    int maxIndex();
    QString productName();

private:
    QString m_bookFilename;
    QString m_bgFilename;
    int m_width;
    int m_height;
    int m_pageIndex;
    int m_previousIndex;
    int m_maxIndex;


signals:
    void bookFilenameChanged();
    void bgFilenameChanged();
    void widthChanged();
    void heightChanged();
    void pageIndexChanged();
};

#endif // BACKEND_H
