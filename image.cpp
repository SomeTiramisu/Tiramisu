#include "image.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>
#include <QImage>

using namespace cv;

Image::Image(char *buf, unsigned int length) {
    img = imdecode(Mat(1, length, CV_8UC1, buf), IMREAD_COLOR);
    createAlpha();
    scale();
};

void Image::createAlpha() {
    cvtColor(img, img, COLOR_BGR2RGBA);
    /*
    int treshold = 230; //245
    for (int y=0; y<img.rows; y++)
    for (int x=0; x<img.cols; x++) {
        Vec4b &pixel = img.at<Vec4b>(y, x);
        if (pixel[0]>= treshold && pixel[1]>= treshold && pixel[2]>=treshold) {
            pixel[3] = 0;
        }
    }*/
}

QPixmap *Image::toQPixmap() {
    QPixmap *r = new QPixmap;
    r->convertFromImage(QImage(img.data, img.cols, img.rows, QImage::Format_RGBA8888));
    return r;
    
};

void Image::scale() {
    double img_width  = static_cast<double>(img.cols);
    double img_height  = static_cast<double>(img.rows);
    double view_width  = static_cast<double>(1200);
    double view_height  = static_cast<double>(1920);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
    if (fx > fy) {
        resize(img, img, Size(), fy, fy);
    } else {
        resize(img, img, Size(), fx, fx);
    }

}
