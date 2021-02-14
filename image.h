#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include <QThread>
#include <queue>
#include "book.h"

using namespace cv;

class ImageProc
{
public:
    ImageProc(char *buf, long long length, std::string bg_filename);
    QPixmap* toQPixmap();
    Mat createMask(Mat& src);
    Rect createROI(Mat* src);
    void createAlpha(Mat* src, Mat* dst);
    void addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst );
    void addBackground(Mat& src, Mat& bg, Mat& dst, Mat& mask);
    void scale(Mat& src, Mat& dst, int view_width, int view_height);
    void scaleFit(Mat* src, Mat* dst, int view_width, int view_height);
    void tileFit(Mat& src, Mat& dst, int view_width, int view_height);
    void sharpen(Mat& src, Mat& dst);
    void process(int width, int height);

private:
    Mat img;
    Mat bg;
};

#endif
