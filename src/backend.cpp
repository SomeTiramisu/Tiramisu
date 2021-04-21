#include "backend.h"
#include "parsers/parser.h"

#include <QStandardPaths>

Backend::Backend() {
    m_init = true;
    m_pageIndex = 0;
    QUrl bgfn("qrc:///res/background.png");
    QUrl bkdr;
    if (QSysInfo::productType() == "android") {
        QStringList locations(QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation));
        if (not locations.empty()) {
            bkdr = QUrl::fromLocalFile(locations[0]);
        } else {
            locations = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
            bkdr = QUrl::fromLocalFile(locations[0]);
        }
    } else {
        QStringList locations = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
        bkdr = QUrl::fromLocalFile(locations[0]);
    }
    setBookDir(bkdr);
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

void Backend::setBookDir(QUrl &d) {
    if (d != m_bookDir) {
        m_bookDir = d;
        emit bookDirChanged();
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

QUrl Backend::bookDir() {
    return m_bookDir;
}

int Backend::pageIndex() {
    return m_pageIndex;
}

QString Backend::getProductName() {
    return QSysInfo::productType();
}

int Backend::getBookSize(QUrl bookFilename) {
    Parser b(bookFilename, false);
    return b.getSize();
}
