#ifndef PAGEPRELOADER_H
#define PAGEPRELOADER_H

#include <thread>
#include <opencv2/core.hpp>
#include "utils/utils.h"
#include "parsers/parser.h"
#include "cropdetectrunner.h"

class PagePreloader
{
public:
    PagePreloader() = default;
    PagePreloader(PagePreloader&&) = default;
    PagePreloader(const Path& filename);
    PagePreloader& operator=(PagePreloader&&) = default;
    PngPair at(int index);
    int size() const;
    Path filename() const;
    int progress() const;

private:
    std::unique_ptr<Parser> m_parser;
    std::vector<CropDetectRunner> m_pages;
    Path m_filename;
    bool isReady{false};
    int m_progress{0};
};

#endif // PAGEPRELOADER_H
