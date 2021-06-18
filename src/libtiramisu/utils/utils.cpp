#include "utils.h"

/*
PageRequest::PageRequest(int width, int height, int index, Path filename)
    : m_width(width),
      m_height(height),
      m_index(index),
      m_filename(filename),
      m_valid(true)
{
}

PageRequest PageRequest::addIndex(int i) const {
    return PageRequest(m_width, m_height, m_index+i, m_filename);
}

void PageRequest::setValid(bool valid) {
    m_valid = valid;
}

bool PageRequest::isLike(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_filename==a.filename());
}

bool PageRequest::operator==(const PageRequest& a) const {
    return (m_width==a.width()
            && m_height==a.height()
            && m_index==a.index()
            && m_filename==a.filename());
}

bool PageRequest::operator!=(const PageRequest& a) const {
    return not operator==(a);
}*/
