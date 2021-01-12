#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include <QThread>
#include <queue>
#include "book.h"

using namespace cv;

class Image
{
public:
    Image(char *buf, unsigned int length);
    QPixmap *toQPixmap();
    Mat createMask(Mat& src);
    Rect createROI(Mat* src);
    void createAlpha(Mat* src, Mat* dst);
    void addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst );
    void addBackground(Mat& src, Mat& bg, Mat& dst, Mat& mask);
    void scale(Mat& src, Mat& dst, int view_width, int view_height);
    void scaleFit(Mat* src, Mat* dst, int view_width, int view_height);
    void tileFit(Mat& src, Mat& dst, int view_width, int view_height);
    void process(int width, int height);

private:
    Mat img;
    Mat bg;
};

#endif
