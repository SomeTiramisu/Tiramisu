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
    void createAlpha();
    void createMask();
    void removeBorders();
    void addBackground();
    void scale(double view_width, double view_height);
private:
    Mat img;
    Mat mask;
};

#endif
