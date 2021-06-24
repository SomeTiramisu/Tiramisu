#ifndef CROPDETECTRUNNER_H
#define CROPDETECTRUNNER_H

#include <thread>
#include "parsers/parser.h"
#include "utils/utils.h"
#include "threadpool.h"

class CropDetectRunner
{
public:
    CropDetectRunner() = default;
    CropDetectRunner(CropDetectRunner&& other) = default;
    CropDetectRunner(Parser* parser, QThreadPool* pool);
    void get(int index, const Slot<PngPair>& slot);
    void get(int index);
    void clear();
    CropDetectRunner& operator=(CropDetectRunner&& other) = default;

private:
    void run();
    static PngPair cropDetect(const ByteVect& png, int index);
    int m_index{-1};
    Parser* m_parser{nullptr};
    QThreadPool* m_pool{nullptr};
    PngPair m_res;
    Slot<PngPair> m_slot;

};

#endif // CROPDETECTRUNNER_H
