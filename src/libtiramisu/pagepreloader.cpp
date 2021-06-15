#include "pagepreloader.h"

#include <fstream>
#include "utils/imageproc.h"

PagePreloader::PagePreloader() {
}

PagePreloader::PagePreloader(const Path& filename)
    : m_filename(filename)
{
    qWarning("preloader created");
    if (m_filename.empty()) {
        return;
    }

    std::ifstream file;
    file.open(m_filename.native(), std::ifstream::binary);
    std::vector<char> ramArchive((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    m_parser =  std::make_unique<Parser>(ramArchive);
    m_pages.resize(m_parser->size());
    for (int i=0; i<m_parser->size(); i++) {
        runCrop(m_parser.get(), i, &m_pages);
    }
}

PagePreloader::~PagePreloader() {
    qWarning("preloader deleted");
}

PngPair PagePreloader::at(int index) {
    return m_pages.at(index);
}

void PagePreloader::runCrop(Parser* parser, int index, std::vector<PngPair>* pages) {
    ByteVect png = parser->at(index);
    cv::Mat img = ImageProc::fromVect(png);
    cv::Rect roi;
    if (not img.empty() and index != 0) {
        roi = ImageProc::cropDetect(img);
    }
    qWarning("CropDetectRunnable: running: %i", index);
    pages->at(index) = PngPair{png, roi};
}

int PagePreloader::size() const {
    return m_pages.size();
}

Path PagePreloader::filename() const {
    return m_filename;
}

int PagePreloader::progress() {
    return m_progress;
}
