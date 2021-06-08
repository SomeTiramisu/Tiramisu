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

Rect ImageProc::createROI(const Mat& src) {
    Rect roi = boundingRect(src); //may be inverted
    if (roi.empty()) { //fix for while images
        roi = Rect(0,0, src.cols, src.rows);
    }
    return roi;
}

void ImageProc::createAlpha(Mat* src, Mat* dst) {
    dst->create(src->cols, src->rows, CV_8UC1);
    int treshold = 235; //245
    for (int y=0; y<dst->rows; y++) {
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
}


void ImageProc::addBackground(const Mat& src, const Mat& bg, Mat& dst, const Mat& mask) {
    double hm = floor(abs(bg.cols - src.cols)/2);
    double vm = floor(abs(bg.rows - src.rows)/2);
    Rect roi = Rect(hm, vm, src.cols, src.rows);
    Mat bg_roi = bg.operator()(roi);
    //qWarning("bg: %i %i %i %i %i %i", bg_roi.cols, bg_roi.rows, src.cols, src.rows, mask.cols, mask.rows);
    src.copyTo(bg_roi, mask);
    bg.copyTo(dst);

}


QPixmap ImageProc::toQPixmap(const Mat& src) {
    if (src.empty()) {
        return QPixmap();
    }
    return QPixmap::fromImage(toQImage(src));
}

QImage ImageProc::toQImage(const Mat& src) {
    if (src.empty()) {
        return QImage();
    }
    size_t length = src.total()*src.elemSize();
    uchar* new_data = new uchar[length];
    std::memcpy(new_data, src.data, length);
    return QImage(new_data, src.cols, src.rows, src.step, QImage::Format_RGBA8888, &Utils::cleanupPageImage, new_data); //src.step is byte per line
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

void ImageProc::scale(const Mat& src, Mat& dst, int view_width, int view_height) {
    int img_width  = src.cols;
    int img_height  = src.rows;
    double fx = static_cast<double>(view_width) / static_cast<double>(img_width);
    double fy = static_cast<double>(view_height) / static_cast<double>(img_height);
    double f = min(fx, fy);
    //qWarning("f: %f", f);
    if (f > 1) {
        resize(src, dst, Size(), f, f, INTER_CUBIC);
    } else {
        resize(src, dst, Size(), f, f, INTER_AREA);
    }
    //qWarning("%i %i", dst.cols, dst.rows);
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

void ImageProc::tileFit(const Mat& src, Mat& dst, int view_width, int view_height) {
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
    for (int y=0; y<dst->rows; y++) {
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
}

void ImageProc::sharpen(const Mat &src, Mat &dst) {
    Mat blurred;
    double sigma = 1;
    double threshold = 5;
    double amount = 1;
    GaussianBlur(src, blurred, Size(), sigma, sigma);
    Mat lowContrastMask = abs(src - blurred) < threshold;
    Mat sharpened = src*(1+amount) + blurred*(-amount);
    dst.copyTo(sharpened, lowContrastMask);
}

void ImageProc::centerFit(const Mat& src, Mat& dst, int view_width, int view_height) {
    double hm = floor(abs(view_width - src.cols)/2);
    double vm = floor(abs(view_height - src.rows)/2);
    Rect roi = Rect(hm, vm, src.cols, src.rows);
    Mat white(view_width, view_height, src.type());
    //white.setTo(Scalar(0, 0, 0));

    Mat bg_roi = white(roi);
    src.copyTo(bg_roi);
    white.copyTo(dst);
}

void ImageProc::classicProcess(const Mat& src, Mat& dst, int width, int height) {
    Mat tmp;
    Mat mask;
    createMask(src, mask);
    Rect roi = createROI(mask);
    //qWarning("img: %i %i, mask: %i %i", src.cols, src.rows, roi.width, roi.height);
    //if (src.cols == roi.width && src.rows == roi.height) {//TODO black and white margin support
    //  createMask(src, mask, true);
    //  roi = createROI(mask);
    //}
    scale3(src(roi), tmp, width, height);
    createMask(tmp, mask);
    tmp.copyTo(dst, mask);
}

void ImageProc::cropProcess(const Mat &src, Mat &dst) {
    Mat mask;
    createMask(src, mask);
    Rect roi = createROI(mask);
    src(roi).copyTo(dst);
}

void ImageProc::scaleProcess(const Mat &src, Mat &dst, int width, int height) {
    Mat tmp;
    Mat mask;
    scale3(src, tmp, width, height);
    createMask(tmp, mask);
    tmp.copyTo(dst, mask);
}

void ImageProc::jpegLosslessCropProcess(QByteArray& src) {
    Mat tmp = fromByteArray(src);
    if (tmp.empty()) {
        qWarning("losslessCrop failed, img is empty");
        return;
    }
    Mat mask;
    createMask(tmp, mask);
    Rect roi = createROI(mask); //Now we have x, y, width, height
    tjtransform xform;
    tjhandle tjInstance = tjInitTransform();
    memset(&xform, 0, sizeof(tjtransform)); //initialise xform to 0
    xform.r.w = roi.width;
    xform.r.h = roi.height;
    xform.r.x = roi.x;
    xform.r.y = roi.y;
    if (xform.r.x < 0 || xform.r.y < 0 || xform.r.w < 1 || xform.r.h < 1) {
        qWarning("losslessCrop failed, the whole img is cropped");
        return;
    }
    xform.options |= TJXOPT_CROP;
    xform.options |= TJXOPT_TRIM;
    uchar *dstBuf = NULL; // TurboJpeg dynamic allocation
    ulong dstSize = 0;
    tjTransform(tjInstance, reinterpret_cast<const uchar*>(src.constData()), src.size(), 1, &dstBuf, &dstSize, &xform, 0);
    if (dstSize == 0) {
        qWarning("0 dstSize");
    } else {
        src.clear();
        src = QByteArray(reinterpret_cast<char*>(dstBuf), dstSize);
    }
    tjFree(dstBuf);
    tjDestroy(tjInstance);
}

Rect ImageProc::cropDetect(const Mat &src) {
    Mat mask;
    createMask(src, mask);
    Rect roi = findBorders(mask);
    if (roi.empty()) { //fix for white images
        roi = Rect(0,0, src.cols, src.rows);
    }
    return roi;
}

void ImageProc::cropScaleProcess(const Mat &src, Mat &dst, const Rect &roi, int width, int height) {
    Mat tmp;
    Mat mask;
    scale3(src(roi), tmp, width, height);
    createMask(tmp, mask);
    tmp.copyTo(dst, mask);
}
