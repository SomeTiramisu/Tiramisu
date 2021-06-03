#ifndef PAGEPRELOADER_H
#define PAGEPRELOADER_H

#include <QByteArray>
#include <QUrl>
#include <QVector>
#include <QObject>

class PagePreloader: public QObject
{
    Q_OBJECT
public:
    PagePreloader(QUrl filename = QUrl(), QObject* parent = nullptr);
    ~PagePreloader() {};
    QByteArray at(int index);
    int size() const;
    QUrl filename() const;

private:
    QVector<QByteArray> m_pages;
    QUrl m_filename;
};

#endif // PAGEPRELOADER_H
