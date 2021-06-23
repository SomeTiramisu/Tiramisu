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

    //std::ifstream file;
    //file.open(m_filename, std::ifstream::binary);
    //std::vector<char> ramArchive((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //file.close();
    //m_parser =  std::make_unique<Parser>(ramArchive);
    m_parser = std::make_unique<Parser>(static_cast<std::string>(filename));

    m_pages.resize(m_parser->size());

    for (int i=0; i<m_parser->size(); i++) {
        m_pages.at(i) = CropDetectRunner(m_parser.get());
        m_pages.at(i).get(i);
        //qWarning("adding %i to preloader.m_pages", i);
    };
}

void PagePreloader::at(int index, const Slot<PngPair>& slot) {
    qWarning("preloader.get(%i)", index);
    m_pages.at(index).get(index, slot);
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
