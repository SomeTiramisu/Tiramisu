#include "pageitem.h"

#include <QPainter>

PageItem::PageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_scheduler = new PageScheduler();
}

PageItem::~PageItem() {
    m_scheduler->deleteLater();
}

void PageItem::setFilename(const QUrl &filename) {
    m_filename = filename;
    if (m_scheduler) {
        m_scheduler->deleteLater();
    }
    m_scheduler = new PageScheduler(filename, 20);
    m_bookSize = Parser(filename).size(); //TODO
    setIndex(0);
    emit filenameChanged();
    emit bookSizeChanged();
}

void PageItem::setIndex(int index) {
    m_index = index;
    PageRequest req(this->width(), this->height(), index, m_filename, 20);
    //PageRequest req(500, 500, index, m_filename, 20);

    if (m_ans) {
        m_ans->deleteLater();
    }
    m_ans = new PageAnswer();
    connect(m_ans, &PageAnswer::s_answer, this, &PageItem::handleDone);
    qWarning("Hello You %i", index);
    m_scheduler->getAsyncPage(req, m_ans);
    //m_image = QImage("/home/guillaume/reader/000.jpg");
    emit indexChanged();
}

void PageItem::paint(QPainter *painter) {
    painter->drawImage(0, 0, m_image);
}

void PageItem::handleDone(QImage img) {
    qWarning("Handled Page");
    m_image = img;
    this->update();
}
