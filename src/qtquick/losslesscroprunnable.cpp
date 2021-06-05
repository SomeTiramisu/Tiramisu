#include "losslesscroprunnable.h"

#include "utils/imageproc.h"

LosslessCropRunnable::LosslessCropRunnable(Parser* parser, int index)
    : m_index(index),
      m_parser(parser)
{}

void LosslessCropRunnable::run() {
    QByteArray png = m_parser->at(m_index);
    ImageProc::jpegLosslessCropProcess(png);
    qWarning("LosslessCropRunnable: running: %i", m_index);
    emit pngReady(m_index, png);
}
