#include "backend.h"

Backend::Backend() {
    m_pageIndex = 0;
}

Backend::~Backend() {
    delete m_book;
}

void Backend::setFilename(QString &f) {
    if (f != m_filename) {
        m_filename = f;
        m_book = new Book(f.toStdString());
        emit filenameChanged();
    }
}

void Backend::setWidth(int &w) {
    if (w != m_width) {
        m_width = w;
        emit widthChanged();
    }
}

void Backend::setHeight(int &h) {
    if (h != m_height) {
        m_height = h;
        emit heightChanged();
    }
}

void Backend::setPageIndex(int &i) {
    if (i>=0 && i<m_book->getSize()) m_pageIndex = i;
};

QString Backend::filename() {
    return m_filename;
}

int Backend::width() {
    return m_width;
}

int Backend::height() {
    return  m_height;
}

int Backend::pageIndex() {
    return m_pageIndex;
}

Book* Backend::book() {
    return m_book;
}
