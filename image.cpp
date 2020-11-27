#include "image.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>
#include <QImage>

using namespace cv;

Image::Image(char *buf, unsigned int length) {
    img = imdecode(Mat(1, length, CV_8UC1, buf), IMREAD_COLOR);
};

void Image::setMask() {
    Mat gs;
    cvtColor(img, gs, COLOR_BGR2GRAY);
    threshold(gs, mask, 235, 255, THRESH_BINARY);

}

void Image::setROI() {
    roi = boundingRect(mask); //may be inverted
    if (roi == Rect(0, 0, 0, 0)) { //fix for while images
            roi = Rect(0,0, img.cols, img.rows);
    }
}

void Image::createAlpha(Mat src, Mat dst) {
    cvtColor(src, dst, COLOR_BGR2RGBA);

    int treshold = 235; //245
    for (int y=0; y<dst.rows; y++)
    for (int x=0; x<dst.cols; x++) {
        Vec4b &pixel = dst.at<Vec4b>(y, x);
        if (pixel[0]>= treshold && pixel[1]>= treshold && pixel[2]>=treshold) {
            pixel[3] = 0;
        }
    }

}


void Image::addBackground(Mat src, Mat bg, Mat dst) {
    Mat bg = imread("/storage/emulated/0/b.png", IMREAD_COLOR);
    scaleFit(bg, bg, 1200, 1920);
    Rect roi = Rect(0, 0, img.cols, img.rows);
    qWarning("%i %i %i %i", bg.cols, bg.rows, img.cols, img.rows);

    //add(bg(roi),img, img, mask);
    //addWeighted(img, alpha, img, 1, 0, img);
    bitwise_and(bg(roi), img, img, mask);
}


QPixmap *Image::toQPixmap() {
    QPixmap *r = new QPixmap;
    r->convertFromImage(QImage(img.data, img.cols, img.rows, QImage::Format_BGR888));
    return r;
    
};

void Image::scale(Mat src, Mat dst, double view_width, double view_height) {
    double img_width  = static_cast<double>(src.cols);
    double img_height  = static_cast<double>(src.rows);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
    double f = min(fx, fy);
    int interpolation;
    if (f > 1) {
        interpolation = INTER_CUBIC;
    } else {
        interpolation = INTER_AREA;
    }
    resize(src, dst, Size(), f, f, interpolation);
}

void Image::scaleFit(Mat src, Mat dst, double view_width, double view_height) {
    double img_width  = static_cast<double>(src.cols);
    double img_height  = static_cast<double>(src.rows);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
    double f = max(fx, fy);
    int interpolation;
    if (f > 1) {
        interpolation = INTER_CUBIC;
    } else {
        interpolation = INTER_AREA;
    }
    resize(src, dst, Size(), f, f, interpolation);
    Rect roi = Rect(0, 0, view_width, view_height);
    dst = dst(roi);
}

void Image::tileFit(Mat src, Mat dst, double view_width, double view_height) {
    int h = 1;
    int v = 1; 
    if (src.cols < view_width) {
        h = view_width / src.cols + 1;
    }
    if (src.rows < view_height) {
        v = view_height / src.rows + 1;
    }
    src.copyTo(dst);
    for (int i=0; i<h; i++) {
        hconcat(dst, src, dst);
    }
    for (int i=0; i<v; i++) {
        vconcat(dst, src, dst);
    }
    Rect roi = Rect(0, 0, view_width, view_height);
    dst = dst(roi);
}

void Image::addAlphaAware(Mat src1, Mat src2, Mat dst ) {
    
    
}


void Image::process(double width, double height) {
    setROI();
    scale(img(roi), img, width, height);
    
    addBackground();
}
