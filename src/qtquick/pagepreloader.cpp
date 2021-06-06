#include "pagepreloader.h"

#include "losslesscroprunnable.h"

PagePreloader::PagePreloader(QUrl filename, QObject* parent)
    : QObject(parent),
      m_filename(filename)
{
    if (m_filename.isEmpty()) {
        return;
    }
    m_parser = new Parser(m_filename, true);
    m_pages.resize(m_parser->size());
    for (int i=0; i<m_parser->size(); i++) {
        runLocalCrop(i);
    }
}

PagePreloader::~PagePreloader() {
    if (m_parser) {
        delete m_parser;
    }
    m_pool.clear();
    m_pool.waitForDone();
    qWarning("preloader deleted");
}

QByteArray PagePreloader::at(int index) {
    return m_pages.at(index);
}

void PagePreloader::runCrop(int index) {
    LosslessCropRunnable* runnable = new LosslessCropRunnable(m_parser, index);
    connect(runnable, &LosslessCropRunnable::pngReady, this, &PagePreloader::handlePng);
    m_pool.start(runnable);
}

void PagePreloader::runLocalCrop(int index) {
    LosslessCropRunnable* runnable = new LosslessCropRunnable(m_parser, index);
    connect(runnable, &LosslessCropRunnable::pngReady, this, &PagePreloader::handlePng);
    runnable->run();
    runnable->autoDelete();
}

int PagePreloader::size() const {
    return m_pages.size();
}

QUrl PagePreloader::filename() const {
    return m_filename;
}

void PagePreloader::handlePng(int index, QByteArray array) {
    m_pages.replace(index, array);
}
