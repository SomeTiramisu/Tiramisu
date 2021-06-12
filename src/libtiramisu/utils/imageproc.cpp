#include "imageproc.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "utils.h"
#include "crop.h"

extern "C" {
#include "turbojpeg.h"
}

using namespace cv;

void ImageProc::createMask(const Mat& src, Mat& dst, bool inv) {
    cvtColor(src, dst, COLOR_RGBA2GRAY);
    if (!inv) {
        threshold(dst, dst, 240, 255, THRESH_BINARY_INV); //235 origin
    } else {
        threshold(dst, dst, 50, 255, THRESH_BINARY);
    }
}

QImage ImageProc::toQImage(const Mat& src) {
    if (src.empty()) {
        return QImage();
    }
    QImage tmp(src.data, src.cols, src.rows, src.step, QImage::Format_RGBA8888);
    return tmp.copy();
}

cv::Mat ImageProc::fromByteArray(const QByteArray& src) {
    if (src.isEmpty()) {
        return  cv::Mat();
    }
    uchar* buf = new uchar[src.length()];
    std::memcpy(buf, src.constData(), src.length());
    cv::Mat img = imdecode(cv::Mat(1, src.length() , CV_8UC1, buf), cv::IMREAD_COLOR);
    delete[] buf;
    cvtColor(img, img, COLOR_BGR2RGBA);
    return img;
}

QByteArray ImageProc::toPng(const Mat &src) {
    if (src.empty()) {
        return QByteArray();
    }
    cv::Mat tmp;
    cvtColor(src, tmp, COLOR_RGBA2BGRA);
    std::vector<uchar> array;
    std::vector<int> params;
    params.push_back(IMWRITE_PNG_COMPRESSION);
    params.push_back(1); //may be 2. 1 is too less
    cv::imencode(".png", tmp, array, params);
    const char* buf = reinterpret_cast<const char*>(array.data());
    return QByteArray(buf, array.size());
}

void ImageProc::scale3(const Mat& src, Mat& dst, int view_width, int view_height) {
    int img_width  = src.cols;
    int img_height  = src.rows;
    double fx = 1.0;
    double fy = 1.0;
    if (view_width==0 && view_height>0) {
        fy = static_cast<double>(view_height) / static_cast<double>(img_height);
        fx = fy;
    }
    if (view_height==0 && view_width>0) {
        fx = static_cast<double>(view_width) / static_cast<double>(img_width);
        fy = fx;
    }
    if (view_width>0 && view_height>0) {
        fx = static_cast<double>(view_width) / static_cast<double>(img_width);
        fy = static_cast<double>(view_height) / static_cast<double>(img_height);
    }
    double f = min(fx, fy);
    if (f > 1) {
        resize(src, dst, Size(), f, f, INTER_CUBIC);
    } else {
        resize(src, dst, Size(), f, f, INTER_AREA);
    }
}

Rect ImageProc::cropDetect(const Mat &src) {
    Mat mask;
    createMask(src, mask);
    return findBorders(mask);
}

void ImageProc::cropScaleProcess(const Mat &src, Mat &dst, const Rect &roi, int width, int height) {
    Mat tmp;
    Mat mask;
    Rect froi(roi);
    if (froi.empty()) { //fix for white images
        froi = Rect(0,0, src.cols, src.rows);
    }
    scale3(src(froi), tmp, width, height);
    createMask(tmp, mask);
    tmp.copyTo(dst, mask);
}
