#include "pageitem.h"

#include <QPainter>

PageItem::PageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, &QTimer::timeout, this, &PageItem::resizeTimeout);
    connect(this, &QQuickItem::widthChanged, this, &PageItem::onRotationChanged);
    connect(this, &QQuickItem::heightChanged, this, &PageItem::onRotationChanged);
}

PageItem::~PageItem() {
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    m_tiramisu.setFilename(filename);
    m_bookSize = m_tiramisu.bookSize(); //TODO
    setIndex(0);
    emit filenameChanged();
    emit bookSizeChanged();
}

void PageItem::setIndex(int index) {
    m_index = index;
    m_req = PageRequest(width(), height(), index, m_filename);
    qWarning("Hello You %i", index);
    //m_image = QImage("/home/guillaume/reader/000.jpg");
    m_image = m_tiramisu.get(m_req);
    this->update();
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
    m_req = PageRequest(width(), height(), m_index, m_filename);
    m_image = m_tiramisu.get(m_req);
    m_tmpImage = QImage();
    this->update();
}
