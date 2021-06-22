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
}

PageItem::~PageItem() {
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    setIndex(0);
    emit filenameChanged();
    emit bookSizeChanged();
}

void PageItem::setIndex(int index) {
    m_index = index;
    m_req = PageRequest{index, (int)width(), (int)height(), m_filename.toLocalFile().toStdString()};
    //qWarning("Hello You %i", index);
    //m_image = QImage("/home/guillaume/reader/000.jpg");
    m_tiramisu.get(m_req, [this](const cv::Mat& img){this->handleSlot(img);});
    //qWarning("ITEM(%i): %i %i",m_index, m_image.width(), m_image.height());
    //this->update();
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
    m_req = PageRequest{m_index, (int)width(), (int)height(), m_filename.toLocalFile().toStdString()};
    m_tiramisu.get(m_req, [this](const cv::Mat& img){this->handleSlot(img);});
    m_tmpImage = QImage();
    //this->update();
}

void PageItem::onImageChanged() {
    qWarning("DEBUG5");
    this->update();
}

QImage toQImage(const cv::Mat& src) {
    if (src.empty()) {
        return QImage();
    }
    QImage tmp(src.data, src.cols, src.rows, src.step, QImage::Format_RGBA8888);
    return tmp.copy();
}

void PageItem::handleSlot(const cv::Mat& img) {
    if(img.empty()) {
            qWarning("DEBUG4 EMPTY");
    } else {
        qWarning("DEBUG4");
    }
    m_image = toQImage(img);
    emit imageChanged();
    //this->update();
}
