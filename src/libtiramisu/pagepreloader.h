#ifndef PAGEPRELOADER_H
#define PAGEPRELOADER_H

#include <opencv2/core.hpp>
#include "utils/utils.h"
#include "parsers/parser.h"

class PagePreloader
{
public:
    PagePreloader();
    PagePreloader(const Path& filename);
    ~PagePreloader();
    PngPair at(int index);
    static void runCrop(Parser*, int, std::vector<PngPair>*);
    int size() const;
    std::filesystem::path filename() const;
    int progress();

private:
    std::unique_ptr<Parser> m_parser;
    std::vector<PngPair> m_pages;
    Path m_filename;
    bool isReady{false};
    int m_progress{0};
};

#endif // PAGEPRELOADER_H
