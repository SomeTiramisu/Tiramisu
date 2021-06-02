#include "utils.h"

PageRequest::PageRequest(int width, int height, int index, QUrl filename, int schedulerPreload)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_filename(filename),
      m_schedulerPreload(schedulerPreload)
{
    static const int typeId = qRegisterMetaType<PageRequest>();
    Q_UNUSED(typeId)
}

PageRequest PageRequest::fromId(const QString& id) {
    //qWarning("id: %s", id.toStdString().c_str());
    QJsonObject jido = QJsonDocument::fromJson(QUrl::fromPercentEncoding(id.toUtf8()).toUtf8()).object(); //boncourage
    return PageRequest(
            jido.value("width").toInt(),
            jido.value("height").toInt(),
            jido.value("index").toInt(),
            jido.value("filename").toString(),
            jido.value("schedulerPreload").toInt()
                );
    //qWarning("DecodeID: decoded: %i, %i, %i, %s, %s", ret.width(), ret.height(), ret.index(), ret.book_filename().toString().toStdString().c_str(), ret.controller_id().toStdString().c_str());
}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_filename, m_schedulerPreload);
}

PageRequest::~PageRequest() {
    //delete m_ans;
}

bool PageRequest::isLike(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_filename==a.filename());
}

bool PageRequest::isInRange(const PageRequest& a, int d) const {
    return (a.index() - d <= m_index) && (m_index <= a.index() + d); //a voir
}

bool PageRequest::operator==(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_index==a.index()
            && m_filename==a.filename()
            && m_schedulerPreload==a.schedulerPreload());
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}
