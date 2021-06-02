#include "classicimagerunnable.h"

#include "utils/imageproc.h"

ClassicImageRunnable::ClassicImageRunnable(Parser &parser, PageRequest req)
    : m_parser(parser),
      m_req(req)
{}

void ClassicImageRunnable::run() {
    cv::Mat img = ImageProc::fromByteArray(m_parser.at(m_req.index()));
    if (not img.empty()) {
        ImageProc::classicProcess(img, img, m_req.width(), m_req.height());
    }
    qWarning("ClassicRunnable: running: %i, (%i, %i)", m_req.index(), m_req.width(), m_req.height());
    emit done(m_req, ImageProc::toQImage(img));
}
