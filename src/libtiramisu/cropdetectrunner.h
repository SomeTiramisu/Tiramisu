#ifndef CROPDETECTRUNNER_H
#define CROPDETECTRUNNER_H

#include <thread>
#include "parsers/parser.h"
#include "utils/utils.h"

class CropDetectRunner
{
public:
    CropDetectRunner() = default;
    CropDetectRunner(CropDetectRunner&& other) = default;
    CropDetectRunner(Parser* parser);
    void get(int index, const Slot<PngPair>& slot);
    void get(int index);
    void clear();
    CropDetectRunner& operator=(CropDetectRunner&& other) = default;

private:
    void run();
    static PngPair cropDetect(const ByteVect& png, int index);
    void handleCropDetect(const PngPair& res);
    int m_index{-1};
    Parser* m_parser{nullptr};
    PngPair m_res;
    std::thread m_thread;
    Slot<PngPair> m_slot;

};

#endif // CROPDETECTRUNNER_H
