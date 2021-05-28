#include "utils.h"

PageRequest::PageRequest(int width, int height, int index, QUrl book_filename, QString controller_id, int controller_preload, QString runnable_type)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_book_filename(book_filename),
      m_controller_id(controller_id),
      m_controller_preload(controller_preload),
      m_runnableType(runnable_type)
{}

PageRequest PageRequest::fromId(const QString& id) {
    //qWarning("id: %s", id.toStdString().c_str());
    QJsonObject jido = QJsonDocument::fromJson(QUrl::fromPercentEncoding(id.toUtf8()).toUtf8()).object(); //boncourage
    return PageRequest(
            jido.value("width").toInt(),
            jido.value("height").toInt(),
            jido.value("index").toInt(),
            jido.value("book_filename").toString(),
            jido.value("controller_id").toString(),
            jido.value("controller_preload").toInt(),
            jido.value("runnable_type").toString()
                );
    //qWarning("DecodeID: decoded: %i, %i, %i, %s, %s", ret.width(), ret.height(), ret.index(), ret.book_filename().toString().toStdString().c_str(), ret.controller_id().toStdString().c_str());
}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_book_filename, m_controller_id, m_controller_preload, m_runnableType);
}

PageRequest::~PageRequest() {
    //delete m_ans;
}

bool PageRequest::isLike(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_book_filename==a.book_filename()
            && m_controller_id==a.controller_id())
            && m_runnableType==a.runnableType();
}

bool PageRequest::isInRange(const PageRequest& a, int d) const {
    return (a.index() - d <= m_index) && (m_index <= a.index() + d); //a voir
}

bool PageRequest::operator==(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_index==a.index()
            && m_book_filename==a.book_filename()
            && m_controller_id==a.controller_id()
            && m_controller_preload==a.controller_preload()
            && m_runnableType==a.runnableType()
            );
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}
