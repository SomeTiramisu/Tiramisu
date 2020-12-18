#include "image.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>
#include <QImage>

//#define BACKGROUND_FILENAME "/home/guillaume/reader/b.png"
#define BACKGROUND_FILENAME "/storage/emulated/0/b.png"

using namespace cv;

Image::Image(char *buf, unsigned int length) {
    img = imdecode(Mat(1, length, CV_8UC1, buf), IMREAD_COLOR);
    bg = imread(BACKGROUND_FILENAME, IMREAD_COLOR);
};

Mat Image::createMask(Mat& src) {
    Mat gs;
    cvtColor(src, gs, COLOR_BGR2GRAY);
    threshold(gs, gs, 235, 255, THRESH_BINARY_INV);
    return gs;

}

Rect Image::createROI(Mat* src) {
    Rect roi = boundingRect(*src); //may be inverted
    if (roi == Rect(0, 0, 0, 0)) { //fix for while images
            roi = Rect(0,0, src->cols, src->rows);
    }
    return roi;
}

void Image::createAlpha(Mat* src, Mat* dst) {
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


void Image::addBackground(Mat& src, Mat& bg, Mat& dst, Mat& mask) {
    double hm = floor(abs(bg.cols - src.cols)/2);
    double vm = floor(abs(bg.rows - src.rows)/2);
    Rect roi = Rect(hm, vm, src.cols, src.rows);
    Mat bg_roi = bg.operator()(roi);
    qWarning("bg: %i %i %i %i %i %i", bg_roi.cols, bg_roi.rows, src.cols, src.rows, mask.cols, mask.rows);
    src.copyTo(bg_roi, mask);
    bg.copyTo(dst);

}


QPixmap *Image::toQPixmap() {
    QPixmap *r = new QPixmap;
    r->convertFromImage(QImage(img.data, img.cols, img.rows, QImage::Format_BGR888));
    return r;
    
};

void Image::scale(Mat& src, Mat& dst, double view_width, double view_height) {
    double img_width  = static_cast<double>(src.cols);
    double img_height  = static_cast<double>(src.rows);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
    double f = min(fx, fy);
    qWarning("f: %f", f);
    int interpolation;
    if (f > 1) {
        interpolation = INTER_CUBIC;
    } else {
        interpolation = INTER_AREA;
    }
    resize(src, dst, Size(), f, f, interpolation);
     qWarning("%i %i", dst.cols, dst.rows);
}

void Image::scaleFit(Mat* src, Mat* dst, double view_width, double view_height) {
    double img_width  = static_cast<double>(src->cols);
    double img_height  = static_cast<double>(src->rows);
    double fx = view_width / img_width;
    double fy = view_height / img_height;
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

void Image::tileFit(Mat& src, Mat& dst, double view_width, double view_height) {
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
    dst = dst.operator()(roi);
}

void Image::addAlphaAware(Mat* src1, Mat* src2, Mat* alpha, Mat* dst ) { //src2 should be background
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


void Image::process(double width, double height) {
    qWarning("img: %i %i", img.cols, img.rows);
    Mat mask = createMask(img);
    //bitwise_not(mask, mask);
    qWarning("mask: %i %i", mask.cols, mask.rows);
    Rect roi = createROI(&mask);
    qWarning("ROI: %i %i", roi.width, roi.height);
    Mat imgROI = img(roi);
    scale(imgROI, img, width, height);
    //Mat bg = imread(BACKGROUND_FILENAME, IMREAD_COLOR);
    //scaleFit(&bg, &bg, 1200, 1920);
    tileFit(bg, bg, width, height);
    mask = createMask(img);
    addBackground(img, bg, img, mask);
}

void ImageWorker::setBook(Book *b) {
    book = b;
}
void ImageWorker::addImage(double width, double height) {
    char* buf = book->getNext();
    unsigned int length = book->getLength();
    Image img = Image(buf, length);
    delete[] buf;
    img.process(width, height);
    emit imageReady(img.toQPixmap());
}
