#ifndef CROPDETECTRUNNER_H
#define CROPDETECTRUNNER_H

#include <thread>
#include "parsers/parser.h"
#include "utils/utils.h"

class CropDetectRunner
{
public:
    CropDetectRunner() = default;
    CropDetectRunner(const CropDetectRunner&) = delete;
    CropDetectRunner(CropDetectRunner&&) = default;
    ~CropDetectRunner();
    void run(Parser* parser, int index);
    PngPair get() const;
    void reset();
    bool valid() const;

private:
    static void cropDetect(Parser* parser, int index, PngPair* res);
    bool m_valid{false};
    //std::mutex m_mut;
    std::unique_ptr<PngPair> m_res;
    std::unique_ptr<std::thread> m_th;

};

#endif // CROPDETECTRUNNER_H
