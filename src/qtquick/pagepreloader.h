#ifndef PAGEPRELOADER_H
#define PAGEPRELOADER_H

#include <QByteArray>
#include <QUrl>
#include <QVector>
#include <QObject>
#include <QThreadPool>
#include <QByteArray>
#include "parsers/parser.h"

class PagePreloader: public QObject
{
    Q_OBJECT
public:
    PagePreloader(QUrl filename = QUrl(), QObject* parent = nullptr);
    ~PagePreloader();
    QByteArray at(int index);
    int size() const;
    QUrl filename() const;
    bool ready() const;

private:
    Parser *m_parser{nullptr};
    QVector<QByteArray> m_pages;
    QUrl m_filename;
    QThreadPool m_pool;
    int m_count{0};

signals:
    void isReady();

public slots:
    void handlePng(int index, QByteArray png);
};

#endif // PAGEPRELOADER_H
