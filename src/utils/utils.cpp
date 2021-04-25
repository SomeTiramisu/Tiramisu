#include "utils.h"

PageRequest::PageRequest(int width, int height, int index, QUrl book_filename, QString controller_id, int controller_preload)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_book_filename(book_filename),
      m_controller_id(controller_id),
      m_controller_preload(controller_preload)
{}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_book_filename, m_controller_id, m_controller_preload);
}

PageRequest::~PageRequest() {
    //delete m_ans;
}

bool PageRequest::isLike(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_book_filename==a.book_filename()
            && m_controller_id==a.controller_id());
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
            );
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}
