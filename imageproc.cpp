#include "imageproc.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>
#include <QImage>

using namespace cv;

void ImageProc::createMask(Mat& src, Mat& dst) {
    cvtColor(src, dst, COLOR_RGBA2GRAY);
    threshold(dst, dst, 235, 255, THRESH_BINARY_INV);
}

Rect ImageProc::createROI(Mat& src) {
    Rect roi = boundingRect(src); //may be inverted
    if (roi == Rect(0, 0, 0, 0)) { //fix for while images
            roi = Rect(0,0, src.cols, src.rows);
    }
    return roi;
}

void ImageProc::createAlpha(Mat* src, Mat* dst) {
    dst->create(src->cols, src->rows, CV_8UC1);
    int treshold = 235; //245
    for (int y=0; y<dst->rows; y++)
    for (int x=0; x<dst->cols; x++) {
        Vec4b &pixel = dst->at<Vec4b>(y, x);
        int& a = dst->at<int>(x,y);
        if (pixel[0]>= treshold && pixel[1]>= treshold && pixel[2]>=treshold) {
            a = 0;
        } else {
            a = 1;
        }
    }

}


void ImageProc::addBackground(Mat& src, Mat& bg, Mat& dst, Mat& mask) {
    double hm = floor(abs(bg.cols - src.cols)/2);
    double vm = floor(abs(bg.rows - src.rows)/2);
    Rect roi = Rect(hm, vm, src.cols, src.rows);
    Mat bg_roi = bg.operator()(roi);
    //qWarning("bg: %i %i %i %i %i %i", bg_roi.cols, bg_roi.rows, src.cols, src.rows, mask.cols, mask.rows);
    src.copyTo(bg_roi, mask);
    bg.copyTo(dst);

}


QPixmap* ImageProc::toQPixmap(Mat& src) {
    QPixmap *r = new QPixmap();
    if (src.empty())
        return r;
    r->convertFromImage(QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGBA8888));
    return r;
}

QImage ImageProc::toQImage(Mat& src) {
    if (src.empty())
        return QImage();
    return QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGBA8888);
}

void ImageProc::scale(Mat& src, Mat& dst, int view_width, int view_height) {
    int img_width  = src.cols;
    int img_height  = src.rows;
    double fx = static_cast<double>(view_width) / static_cast<double>(img_width);
    double fy = static_cast<double>(view_height) / static_cast<double>(img_height);
    double f = min(fx, fy);
    //qWarning("f: %f", f);
    int interpolation;
    if (f > 1) {
        interpolation = INTER_CUBIC;
    } else {
        interpolation = INTER_AREA;
    }
    resize(src, dst, Size(), f, f, interpolation);
     //qWarning("%i %i", dst.cols, dst.rows);
/*
    if (dst.cols > view_width) {
        dst = dst(Rect(0, 0, view_width, dst.rows));
    }
    if (dst.rows > view_height) {
        dst = dst(Rect(0, 0, dst.cols, view_height));
    }*/
}

void ImageProc::scaleFit(Mat* src, Mat* dst, int view_width, int view_height) {
    int img_width  = src->cols;
    int img_height  = src->rows;
    double fx = static_cast<double>(view_width) / static_cast<double>(img_width);
    double fy = static_cast<double>(view_height) / static_cast<double>(img_height);
    double f = max(fx, fy);
    int interpolation;
    if (f > 1) {
        interpolation = INTER_CUBIC;
    } else {
        interpolation = INTER_AREA;
    }
    resize(*src, *dst, Size(), f, f, interpolation);
    Rect roi = Rect(0, 0, view_width, view_height);
    *dst = dst->operator()(roi);
}

void ImageProc::tileFit(Mat& src, Mat& dst, int view_width, int view_height) {
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
        vconcat(dst, dst, dst);
    }
    Rect roi = Rect(0, 0, view_width, view_height);
    dst = dst.operator()(roi);
}

void ImageProc::addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst ) { //src2 should be background
    dst->create(src1->cols, src1->rows, CV_8UC3);
    for (int y=0; y<dst->rows; y++)
    for (int x=0; x<dst->cols; x++) {
        Vec4b &pixel_s1 = src1->at<Vec4b>(y, x);
        Vec4b &pixel_s2 = src2->at<Vec4b>(y, x);
        double a = alpha->at<double>(x, y)/255;
        Vec4b &pixel = dst->at<Vec4b>(y, x);
        pixel[0] = pixel_s1[0]*a + pixel_s2[0]*(1-a);
        pixel[1] = pixel_s1[1]*a + pixel_s2[1]*(1-a);
        pixel[2] = pixel_s1[2]*a + pixel_s2[2]*(1-a);
    }
    
}

void ImageProc::sharpen(Mat &src, Mat &dst) {
    Mat blurred;
    double sigma = 1;
    double threshold = 5;
    double amount = 1;
    GaussianBlur(src, blurred, Size(), sigma, sigma);
    Mat lowContrastMask = abs(src - blurred) < threshold;
    Mat sharpened = src*(1+amount) + blurred*(-amount);
    dst.copyTo(sharpened, lowContrastMask);
}

void ImageProc::centerFit(Mat& src, Mat& dst, int view_width, int view_height) {
    double hm = floor(abs(view_width - src.cols)/2);
    double vm = floor(abs(view_height - src.rows)/2);
    Rect roi = Rect(hm, vm, src.cols, src.rows);
    Mat white(view_width, view_height, src.type());
    //white.setTo(Scalar(0, 0, 0));

    Mat bg_roi = white(roi);
    src.copyTo(bg_roi);
    white.copyTo(dst);
}

void ImageProc::classicProcess(Mat& src, Mat& dst, int width, int height) { //src2 is background
    Mat img;
    Mat mask;
    img = src;
    cvtColor(img, img, COLOR_BGR2RGBA);
    createMask(src, mask);
    Rect roi = createROI(mask);
    img = img(roi);
    scale(img, img, width, height);
    createMask(img, mask);
    img.copyTo(dst, mask);
}
