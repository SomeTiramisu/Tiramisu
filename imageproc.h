#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include "book.h"

using namespace cv;

class ImageProc
{
public:
    static QPixmap* toQPixmap(Mat& src);
    static void createMask(Mat& src, Mat& dst);
    static Rect createROI(Mat& src);
    static void createAlpha(Mat* src, Mat* dst);
    static void addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst );
    static void addBackground(Mat& src, Mat& bg, Mat& dst, Mat& mask);
    static void scale(Mat& src, Mat& dst, int view_width, int view_height);
    static void scaleFit(Mat* src, Mat* dst, int view_width, int view_height);
    static void tileFit(Mat& src, Mat& dst, int view_width, int view_height);
    static void sharpen(Mat& src, Mat& dst);
    static void classicProcess(Mat& src, Mat& src2, Mat& dst, int width, int height);
};

#endif
