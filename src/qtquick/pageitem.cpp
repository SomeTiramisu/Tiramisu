#include "pageitem.h"

#include <QPainter>

PageItem::PageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_preloader = new PagePreloader();
    m_scheduler = new PageScheduler(m_preloader);
}

PageItem::~PageItem() {
    m_scheduler->deleteLater();
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    if (m_scheduler) {
        m_scheduler->deleteLater();

    }
    if (m_preloader) {
        m_preloader->deleteLater();
    }
    m_preloader = new PagePreloader(filename);
    connect(m_preloader, &PagePreloader::isReady, this, &PageItem::preloaderReady);
    m_scheduler = new PageScheduler(m_preloader);
    connect(m_scheduler, &PageScheduler::imageReady, this, &PageItem::handleImage);
    m_bookSize = m_preloader->size(); //TODO
    //setIndex(0); now when preloader is ready
    emit filenameChanged();
    emit bookSizeChanged();
}

void PageItem::setIndex(int index) {
    if (not m_preloader->ready()) {
        return;
    }
    m_index = index;
    m_req = PageRequest(width(), height(), index, m_filename);;
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

void PageItem::handleImage(PageRequest req, QImage img) {
    if (req == m_req) {
        qWarning("Handled Page");
        m_image = img;
        this->update();
    }
}

void PageItem::preloaderReady() {
    setIndex(0);
}
