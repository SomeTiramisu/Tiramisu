#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "book.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)

public:
    Backend();
    ~Backend();
    void setFilename(QString &f);
    void setWidth(int &w);
    void setHeight(int &h);
    void setPageIndex(int &i);
    QString filename();
    int width();
    int height();
    int pageIndex();
    Book* book();

private:
    QString m_filename;
    int m_width;
    int m_height;
    int m_pageIndex;
    Book *m_book;


signals:
    void filenameChanged();
    void widthChanged();
    void heightChanged();
    void pageIndexChanged();
};

#endif // BACKEND_H
