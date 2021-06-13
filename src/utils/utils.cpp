#include "utils.h"

PageRequest::PageRequest(int width, int height, int index, QUrl filename, QString schedulerId, int schedulerPreload, QString runnableType)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_filename(filename),
      m_schedulerId(schedulerId),
      m_schedulerPreload(schedulerPreload),
      m_runnableType(runnableType)
{}

PageRequest PageRequest::fromId(const QString& id) {
    //qWarning("id: %s", id.toStdString().c_str());
    QJsonObject jido = QJsonDocument::fromJson(QUrl::fromPercentEncoding(id.toUtf8()).toUtf8()).object(); //boncourage
    return PageRequest(
            jido.value("width").toInt(),
            jido.value("height").toInt(),
            jido.value("index").toInt(),
            jido.value("filename").toString(),
            jido.value("schedulerId").toString(),
            jido.value("schedulerPreload").toInt(),
            jido.value("runnableType").toString()
                );
    //qWarning("DecodeID: decoded: %i, %i, %i, %s, %s", ret.width(), ret.height(), ret.index(), ret.book_filename().toString().toStdString().c_str(), ret.controller_id().toStdString().c_str());
}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_filename, m_schedulerId, m_schedulerPreload, m_runnableType);
}

PageRequest::~PageRequest() {
    //delete m_ans;
}

bool PageRequest::isLike(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_filename==a.filename()
            && m_schedulerId==a.schedulerId())
            && m_runnableType==a.runnableType();
}

bool PageRequest::isInRange(const PageRequest& a, int d) const {
    return (a.index() - d <= m_index) && (m_index <= a.index() + d); //a voir
}

bool PageRequest::operator==(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_index==a.index()
            && m_filename==a.filename()
            && m_schedulerId==a.schedulerId()
            && m_schedulerPreload==a.schedulerPreload()
            && m_runnableType==a.runnableType()
            );
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}
