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
    removeBorders();
    createMask();
    //addBackground();
    scale(1200, 1920);
};

void Image::createAlpha() {
    cvtColor(img, img, COLOR_BGR2RGBA);

    int treshold = 235; //245
    for (int y=0; y<img.rows; y++)
    for (int x=0; x<img.cols; x++) {
        Vec4b &pixel = img.at<Vec4b>(y, x);
        if (pixel[0]>= treshold && pixel[1]>= treshold && pixel[2]>=treshold) {
            pixel[3] = 0;
        }
    }
}

void Image::createMask() {
    Mat gs;
    cvtColor(img, gs, COLOR_BGR2GRAY);
    threshold(gs, mask, 235, 255, THRESH_BINARY);

}

void Image::removeBorders() {
    Mat gs;
    Mat b;
    cvtColor(img, gs, COLOR_BGR2GRAY);
    threshold(gs, b, 235, 255, THRESH_BINARY_INV);
    Rect roi = boundingRect(b);
    if (roi != Rect(0, 0, 0, 0)) { //fix for while images
            img = img(roi);
    }
}

void Image::addBackground() {
    Mat bg = imread("/storage/emulated/0/b.png", IMREAD_COLOR);
    Rect roi = Rect(0, 0, img.cols, img.rows);
    bg = bg(roi);
    add(img, bg, img, mask);
}


QPixmap *Image::toQPixmap() {
    QPixmap *r = new QPixmap;
    r->convertFromImage(QImage(img.data, img.cols, img.rows, QImage::Format_RGBA8888));
    return r;
    
};

void Image::scale(double view_width, double view_height) {
    double img_width  = static_cast<double>(img.cols);
    double img_height  = static_cast<double>(img.rows);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
    if (fx > fy) {
        resize(img, img, Size(), fy, fy);
    } else {
        resize(img, img, Size(), fx, fx);
    }

}
