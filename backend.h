#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUrl>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl bookFilename READ bookFilename WRITE setBookFilename NOTIFY bookFilenameChanged)
    Q_PROPERTY(QUrl bookDir READ bookDir WRITE setBookDir NOTIFY bookDirChanged)
    Q_PROPERTY(QUrl bgFilename READ bgFilename WRITE setBgFilename NOTIFY bgFilenameChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    //Q_PROPERTY(QString productName READ productName CONSTANT)

public:
    Backend();
    ~Backend();
    void setBookFilename(QUrl &f);
    void setBgFilename(QUrl &f);
    void setBookDir(QUrl &d);
    void setPageIndex(int &i);
    QUrl bookFilename();
    QUrl bgFilename();
    QUrl bookDir();
    int pageIndex();
    Q_INVOKABLE QString getProductName();
    QUrl bookFromId(int id);
    Q_INVOKABLE int getMaxIndex(QUrl bookFilename);

private:
    QUrl m_bookFilename;
    QUrl m_bgFilename;
    QUrl m_bookDir;
    int m_pageIndex;
    bool m_init;


signals:
    void bookFilenameChanged();
    void bgFilenameChanged();
    void bookDirChanged();
    void pageIndexChanged();
};

#endif // BACKEND_H
