#include "backend.h"
#include "book.h"

//#define ARCHIVE_FILENAME "/home/guillaume/reader/b.cbr"
//#define BACKGROUND_FILENAME "/home/guillaume/reader/b.png"
//#define ARCHIVE_FILENAME "/storage/emulated/0/b.cbr"
//#define BACKGROUND_FILENAME "/storage/emulated/0/b.png"

Backend::Backend() {
    m_init = true;
    m_pageIndex = 0;
    QUrl bkfn(QUrl::fromLocalFile(ARCHIVE_FILENAME));
    QUrl bgfn(QUrl::fromLocalFile(BACKGROUND_FILENAME));
    //setBookFilename(bkfn);
    setBgFilename(bgfn);
}

Backend::~Backend() {
}

void Backend::setBookFilename(QUrl &f) {
    if (f != m_bookFilename) {
        m_bookFilename = f;
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

void Backend::setPageIndex(int &i) {
    m_pageIndex = i;
    emit pageIndexChanged();
};

QUrl Backend::bookFilename() {
    return m_bookFilename;
}

QUrl Backend::bgFilename() {
    return m_bgFilename;
}

int Backend::pageIndex() {
    return m_pageIndex;
}

QString Backend::productName() {
    return QSysInfo::productType();
}

QUrl Backend::bookFromId(int id) {// TODO
    return bookFilename();
}
