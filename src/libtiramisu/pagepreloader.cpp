#include "pagepreloader.h"

#include "runnables/cropdetectrunnable.h"

PagePreloader::PagePreloader(QUrl filename, QObject* parent)
    : QObject(parent),
      m_filename(filename)
{
    qWarning("preloader created");
    if (m_filename.isEmpty()) {
        return;
    }
    m_parser = new Parser(m_filename, true);
    m_pages.resize(m_parser->size());
    for (int i=0; i<m_parser->size(); i++) {
        runCrop(i);
    }
}

PagePreloader::~PagePreloader() {
    m_pool.clear();
    m_pool.waitForDone();
    if (m_parser) {
        delete m_parser;
    }
    qWarning("preloader deleted");
}

PngPair PagePreloader::at(int index) {
    m_pool.waitForDone(); //here because at() is called from another thread
    return m_pages.at(index);
}

void PagePreloader::runCrop(int index) {
    CropDetectRunnable* runnable = new CropDetectRunnable(m_parser, index);
    connect(runnable, &CropDetectRunnable::roiReady, this, &PagePreloader::handleRoi);
    m_pool.start(runnable);
}

void PagePreloader::runLocalCrop(int index) {
    CropDetectRunnable* runnable = new CropDetectRunnable(m_parser, index);
    connect(runnable, &CropDetectRunnable::roiReady, this, &PagePreloader::handleRoi);
    runnable->run();
    runnable->deleteLater();
}

int PagePreloader::size() const {
    return m_pages.size();
}

QUrl PagePreloader::filename() const {
    return m_filename;
}

int PagePreloader::progress() {
    return m_progress;
}

void PagePreloader::handleRoi(int index, QByteArray png, cv::Rect roi) {
    m_pages.replace(index, PngPair{png, roi});
    m_progress++;
    emit progressChanged();
    if (m_progress==size()) {
        delete m_parser;
        m_parser = nullptr;
        qWarning("preload Parser deleted");
    }
}
