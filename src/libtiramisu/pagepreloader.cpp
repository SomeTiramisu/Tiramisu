#include "pagepreloader.h"

#include <fstream>
#include "utils/imageproc.h"

PagePreloader::PagePreloader(const Path& filename)
    : m_filename(filename)
{
    qWarning("preloader created");
    if (m_filename.empty()) {
        return;
    }

    std::ifstream file;
    file.open(m_filename, std::ifstream::binary);
    std::vector<char> ramArchive((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    m_parser =  std::make_unique<Parser>(ramArchive);
    m_pages.resize(m_parser->size());

    for (int i=0; i<m_parser->size(); i++) {
        m_pages.at(i) = CropDetectRunner(m_parser.get());
        m_pages.at(i).run(i);
        qWarning("adding %i to preloader.m_pages", i);
    };
}

PngPair PagePreloader::at(int index) {
    qWarning("preloader.get(%i)", index);
    return m_pages.at(index).get(index);
}

int PagePreloader::size() const {
    return m_pages.size();
}

Path PagePreloader::filename() const {
    return m_filename;
}

int PagePreloader::progress() const {
    return m_progress;
}
