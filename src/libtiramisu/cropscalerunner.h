#ifndef CROPSCALERUNNER_H
#define CROPSCALERUNNER_H

#include <optional>
#include "parsers/parser.h"
#include "utils/utils.h"

class CropScaleRunner
{
public:
    CropScaleRunner() = default;
    CropScaleRunner(CropScaleRunner&& other) = default;
    CropScaleRunner(Parser* parser, QThreadPool* pool);
    void get(const PageRequest& req);
    void preload(const PageRequest& req);
    void clear();
    void connectSlot(const Slot<PagePair>& slot);
    CropScaleRunner& operator=(CropScaleRunner&&) = default;

private:
    void runScale();
    void runDetect();
    static PagePair cropScale(const PngPair& p, const PageRequest& req);
    static PngPair cropDetect(const ByteVect& png, int index);
    PageRequest m_req;
    QThreadPool* m_pool{nullptr};
    Parser* m_parser{nullptr};
    std::optional<PagePair> m_pageRes;
    std::optional<PngPair> m_pngRes;
    Slot<PagePair> m_slot;

};

#endif // CROPSCALERUNNER_H
