#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/core.hpp>
#include <QPixmap>

using namespace cv;

class Image
{
public:
    Image(char *buf, unsigned int length);
    QPixmap *toQPixmap();
    Mat createMask(Mat* src);
    Rect createROI(Mat* src);
    void createAlpha(Mat* src, Mat* dst);
    void addAlphaAware(Mat* src1, Mat* src2, Mat* dst );
    void addBackground(Mat* src, Mat* bg, Mat* dst, Mat* mask);
    void scale(Mat* src, Mat* dst, double view_width, double view_height);
    void scaleFit(Mat* src, Mat* dst, double view_width, double view_height);
    void tileFit(Mat* src, Mat* dst, double view_width, double view_height);
    void process(double width, double height);

private:
    Mat img;
};

#endif
