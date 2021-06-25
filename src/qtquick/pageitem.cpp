#include "pageitem.h"

#include <QPainter>
#include <opencv2/core.hpp>

PageItem::PageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, &QTimer::timeout, this, &PageItem::resizeTimeout);
    connect(this, &QQuickItem::widthChanged, this, &PageItem::onRotationChanged);
    connect(this, &QQuickItem::heightChanged, this, &PageItem::onRotationChanged);
    connect(this, &PageItem::imageChanged, this, &PageItem::onImageChanged);
    m_tiramisu.connectBookSize([this](int bookSize){
        this->m_bookSize = bookSize;
        emit bookSizeChanged();
    });
    m_tiramisu.connectImage([this](const cv::Mat& img){
        //qWarning("DEBUG4");
        this->m_image =toQImage(img);
        emit imageChanged(); //workaround to call update from GUI thread
    });
    m_tiramisu.connectPreloaderProgress([this](int preloaderProgress){
        this->m_preloaderProgress = preloaderProgress;
        emit preloaderProgressChanged();
    });
}

PageItem::~PageItem() {
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    setIndex(0);
    emit filenameChanged();
}

void PageItem::setIndex(int index) {
    m_index = index;
    m_req = PageRequest{index, (int)width(), (int)height(), m_filename.toLocalFile().toStdString()};
    //qWarning("Hello You %i", index);
    //m_image = QImage("/home/guillaume/reader/000.jpg");
    m_tiramisu.get(m_req);
    //qWarning("ITEM(%i): %i %i",m_index, m_image.width(), m_image.height());
    emit indexChanged();
}

void PageItem::paint(QPainter *painter) {
    int x = width()/2 - m_image.width()/2;
    int y = height()/2 - m_image.height()/2;
    painter->drawImage(x, y, m_image);
}

void PageItem::onRotationChanged() {
    if (m_tmpImage.isNull()) {
        m_tmpImage = m_image;
    }
    m_image = m_tmpImage.scaled(size().toSize(), Qt::KeepAspectRatio);
    this->update();
    m_resizeTimer.start(100);
}

void PageItem::resizeTimeout() {
    //qWarning("TIMEOUT");
    m_req = PageRequest{m_index, (int)width(), (int)height(), m_filename.toLocalFile().toStdString()};
    m_tiramisu.get(m_req);
    m_tmpImage = QImage();
}

void PageItem::onImageChanged() {
    //qWarning("DEBUG5");
    this->update();
}

QImage toQImage(const cv::Mat& src) {
    if (src.empty()) {
        return QImage();
    }
    QImage tmp(src.data, src.cols, src.rows, src.step, QImage::Format_RGBA8888);
    return tmp.copy();
}
