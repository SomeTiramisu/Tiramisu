#include "pagepreloader.h"

#include "parsers/parser.h"

PagePreloader::PagePreloader(QUrl filename, QObject* parent)
    : QObject(parent),
      m_filename(filename)
{
    if (m_filename.isEmpty()) {
        return;
    }
    Parser parser(m_filename);
    m_pages.reserve(parser.size());
    for (int i=0; i<parser.size(); i++) {
        m_pages.append(parser.at(i));
    }
}

QByteArray PagePreloader::at(int index) {
    return m_pages.at(index);
}

int PagePreloader::size() const {
    return m_pages.size();
}

QUrl PagePreloader::filename() const {
    return m_filename;
}
