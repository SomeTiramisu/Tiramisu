#include "pageitem.h"

#include <QPainter>

PageItem::PageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_preloader = new PagePreloader();
    m_scheduler = new PageScheduler(m_preloader);

    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, &QTimer::timeout, this, &PageItem::resizeTimeout);
    connect(this, &QQuickItem::widthChanged, this, &PageItem::onRotationChanged);
    connect(this, &QQuickItem::heightChanged, this, &PageItem::onRotationChanged);
}

PageItem::~PageItem() {
    m_scheduler->deleteLater();
    m_preloader->deleteLater();
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    m_scheduler->deleteLater();
    m_preloader->deleteLater();
    m_preloader = new PagePreloader(filename);
    m_scheduler = new PageScheduler(m_preloader);
    connect(m_preloader, &PagePreloader::progressChanged, this, [this]{emit preloaderProgressChanged();});
    connect(m_scheduler, &PageScheduler::imageReady, this, &PageItem::handleImage);
    m_bookSize = m_preloader->size(); //TODO
    setIndex(0);
    emit filenameChanged();
    emit bookSizeChanged();
}

void PageItem::setIndex(int index) {
    m_index = index;
    m_req = PageRequest(width(), height(), index, m_filename);
    qWarning("Hello You %i", index);
    m_scheduler->getAsyncPage(m_req);
    //m_image = QImage("/home/guillaume/reader/000.jpg");
    emit indexChanged();
}

void PageItem::paint(QPainter *painter) {
    int x = width()/2 - m_image.width()/2;
    int y = height()/2 - m_image.height()/2;
    painter->drawImage(x, y, m_image);
}

void PageItem::onRotationChanged() {
    //if(m_index >= m_preloader->size()) { //size = 0
    //    return;
    //}
    //QByteArray png = m_preloader->at(m_index).png;
    //m_image = QImage::fromData(png).scaled(size().toSize(), Qt::KeepAspectRatio);
    if (m_tmpImage.isNull()) {
        m_tmpImage = m_image;
    }
    m_image = m_tmpImage.scaled(size().toSize(), Qt::KeepAspectRatio);
    this->update();
    m_resizeTimer.start(100);
}

void PageItem::resizeTimeout() {
    m_req = PageRequest(width(), height(), m_index, m_filename);
    m_scheduler->getAsyncPage(m_req);
    m_tmpImage = QImage();
}

void PageItem::handleImage(PageRequest req, QImage img) {
    if (req == m_req) {
        qWarning("Handled Page");
        m_image = img;
        this->update();
    }
}
