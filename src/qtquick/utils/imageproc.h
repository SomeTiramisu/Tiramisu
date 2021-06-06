#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include <QImage>

using namespace cv;

class ImageProc
{
public:
    static QPixmap toQPixmap(const Mat& src);
    static QImage toQImage(const Mat& src);
    static cv::Mat fromByteArray(const QByteArray& src); //RGBA colors
    static QByteArray toPng(const Mat& src); //assuming RGBA colors
    static void createMask(const Mat& src, Mat& dst, bool inv=false);
    static Rect createROI(const Mat& src);
    static void createAlpha(Mat* src, Mat* dst);
    static void addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst );
    static void addBackground(const Mat& src, const Mat& bg, Mat& dst, const Mat& mask);
    static void scale(const Mat& src, Mat& dst, int view_width, int view_height);
    static void scale3(const Mat& src, Mat& dst, int view_width, int view_height);
    static void scaleFit(Mat* src, Mat* dst, int view_width, int view_height);
    static void tileFit(const Mat& src, Mat& dst, int view_width, int view_height);
    static void sharpen(const Mat& src, Mat& dst);
    static void centerFit(const Mat& src, Mat& dst, int view_width, int view_height);
    static void classicProcess(const Mat& src, Mat& dst, int width, int height); //assuming color RGBA
    static void cropProcess(const Mat& src, Mat& dst); //assuming color RGBA
    static void scaleProcess(const Mat& src, Mat& dst, int width, int height); //assuming color RGBA
    static void jpegLosslessCropProcess(QByteArray& src);
    static Rect cropDetect(const Mat& src);
    static  void cropScaleProcess(const Mat& src, Mat& dst, const Rect& roi, int width, int height);
};

#endif
