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
    void createMask();
    void removeBorders();
    void addBackground();
    void scale(Mat src, Mat dst, double view_width, double view_height);
    Mat scaleFit(Mat src, Mar dst, double view_width, double view_height);
    void tileFit(Mat src, Mar dst, double view_width, double view_height);
    void process(double width, double height);
    
private:
    Mat img;
    Mat mask;
    Rect roi;
};

#endif
