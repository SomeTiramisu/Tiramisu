#include "backend.h"

#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
#define BACKGROUND_FILENAME "/home/guillaume/reader/b.png"
//#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"
//#define BACKGROUND_FILENAME "/storage/emulated/0/b.png"

Backend::Backend() {
    m_pageIndex = 0;
    QString bkfn(ARCHIVE_FILENAME);
    QString bgfn(BACKGROUND_FILENAME);
    setBookFilename(bkfn);
    setBgFilename(bgfn);
}

Backend::~Backend() {
    delete m_book;
}

void Backend::setBookFilename(QString &f) {
    if (f != m_bookFilename) {
        m_bookFilename = f;
        m_book = new Book(f.toStdString());
        emit bookFilenameChanged();
    }
}

void Backend::setBgFilename(QString &f) {
    if (f != m_bgFilename) {
        m_bgFilename = f;
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
    if (i>=0 && i<m_book->getSize()) m_pageIndex = i;
};

QString Backend::bookFilename() {
    return m_bookFilename;
}

QString Backend::bgFilename() {
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

QString Backend::productName() {
    return QSysInfo::productType();
}

Book* Backend::book() {
    return m_book;
}
