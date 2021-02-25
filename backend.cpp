#include "backend.h"
#include "book.h"

//#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
//#define BACKGROUND_FILENAME "/home/guillaume/reader/b.png"
//#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"
//#define BACKGROUND_FILENAME "/storage/emulated/0/b.png"

Backend::Backend() {
    m_init = true;
    m_pageIndex = 0;
    m_maxIndex = 0;
    QUrl bkfn(QUrl::fromLocalFile(ARCHIVE_FILENAME));
    QUrl bgfn(QUrl::fromLocalFile(BACKGROUND_FILENAME));
    setBookFilename(bkfn);
    setBgFilename(bgfn);
    m_width = 1200; //tofix
    m_height = 1920;
}

Backend::~Backend() {
}

void Backend::setBookFilename(QUrl &f) {
    if (f != m_bookFilename) {
        m_bookFilename = f;
        Book b = Book(f);
        m_maxIndex = b.getSize()-1;
        m_pageIndex = 0;
        m_init = true;
        emit bookFilenameChanged();
    }
}

void Backend::setBgFilename(QUrl &f) {
    if (f != m_bgFilename) {
        m_bgFilename = f;
        m_init = true;
        emit bgFilenameChanged();
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
    if (i>=0 && i<=m_maxIndex) {
        m_pageIndex = i;
        emit pageIndexChanged();
    }
};

QUrl Backend::bookFilename() {
    return m_bookFilename;
}

QUrl Backend::bgFilename() {
    return m_bgFilename;
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

int Backend::maxIndex() {
    return m_maxIndex;
}

QString Backend::productName() {
    return QSysInfo::productType();
}

bool Backend::init() {
    if (m_init) {
        m_init = false;
        return true;
    }
    return false;
}
