#include "utils.h"

PageRequest::PageRequest(int width, int height, int index, QUrl filename)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_filename(filename)
{
    static const int typeId = qRegisterMetaType<PageRequest>();
    Q_UNUSED(typeId)
}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_filename);
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
            && m_filename==a.filename());
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}
