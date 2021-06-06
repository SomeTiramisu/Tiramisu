#include "pagepreloader.h"

#include "runnables/cropdetectrunnable.h"

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
    delete m_parser; //TODO
}

PagePreloader::~PagePreloader() {
    if (m_parser) {
        delete m_parser;
    }
    m_pool.clear();
    m_pool.waitForDone();
    qWarning("preloader deleted");
}

PngPair PagePreloader::at(int index) {
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

void PagePreloader::handleRoi(int index, QByteArray png, cv::Rect roi) {
    m_pages.replace(index, PngPair{png, roi});
}
