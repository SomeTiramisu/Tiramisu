#include "pagepreloader.h"

#include "croprunnable.h"

PagePreloader::PagePreloader(QUrl filename, QObject* parent)
    : QObject(parent),
      m_filename(filename)
{
    if (m_filename.isEmpty()) {
        return;
    }
    m_parser = new Parser(m_filename);
    m_pages.resize(m_parser->size());
    m_count = m_parser->size();
    for (int i=0; i<m_parser->size(); i++) {
        CropRunnable* runnable = new CropRunnable(m_parser, i);
        connect(runnable, &CropRunnable::pngReady, this, &PagePreloader::handlePng);
        m_pool.start(runnable);
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

int PagePreloader::size() const {
    return m_pages.size();
}

QUrl PagePreloader::filename() const {
    return m_filename;
}

bool PagePreloader::ready() const {
    return m_count == 0;
}

void PagePreloader::handlePng(int index, QByteArray array) {
    m_pages.replace(index, array);
    m_count--;
    if (m_count == 0) {
        emit isReady();
    }
}
