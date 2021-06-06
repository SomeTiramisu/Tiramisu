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
    void runCrop(int index);
    void runLocalCrop(int index);
    int size() const;
    QUrl filename() const;

private:
    Parser *m_parser{nullptr};
    QVector<QByteArray> m_pages;
    QUrl m_filename;
    QThreadPool m_pool;
    bool isReady{false};

public slots:
    void handlePng(int index, QByteArray png);
};

#endif // PAGEPRELOADER_H
