#ifndef CROPDETECTRUNNER_H
#define CROPDETECTRUNNER_H

#include <future>
#include "parsers/parser.h"
#include "utils/utils.h"

class CropDetectRunner
{
public:
    CropDetectRunner() = default;
    CropDetectRunner(CropDetectRunner&& other) = default;
    CropDetectRunner(Parser* parser);
    void run(int index);
    PngPair get(int index);
    void clear();
    CropDetectRunner& operator=(CropDetectRunner&& other) = default;

private:
    static PngPair cropDetect(Parser* parser, int index);
    int m_index{0};
    Parser* m_parser{nullptr};
    PngPair m_res;
    std::future<PngPair> m_future;

};

#endif // CROPDETECTRUNNER_H
