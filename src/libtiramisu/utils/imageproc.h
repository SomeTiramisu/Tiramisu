#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include <QImage>

using namespace cv;

class ImageProc
{
public:
    static QImage toQImage(const Mat& src);
    static cv::Mat fromByteArray(const QByteArray& src); //RGBA colors
    static QByteArray toPng(const Mat& src); //assuming RGBA colors
    static void createMask(const Mat& src, Mat& dst, bool inv=false);
    static void scale3(const Mat& src, Mat& dst, int view_width, int view_height);
    static Rect cropDetect(const Mat& src);
    static void cropScaleProcess(const Mat& src, Mat& dst, const Rect& roi, int width, int height);
};

#endif
