// Adapted from https://github.com/tachiyomiorg/image-decoder
//
// Created by len on 25/12/20.
//

#include "crop.h"

using namespace cv;

bool inline isBlackPixel(const uchar* pixels, uint width, uint x, uint y) {
    const uchar pixel = *((uchar *)pixels + (y * width + x));
    return pixel < thresholdForBlack;
}

bool inline isWhitePixel(const uchar* pixels, uint width, uint x, uint y) {
    const uchar pixel = *((uchar *)pixels + (y * width + x));
    return pixel > thresholdForWhite;
}

/** Return the first x position where there is a substantial amount of fill,
 * starting the search from the left. */
uint findBorderLeft(const Mat& src, uint top, uint bottom) {
    uchar* pixels = src.data;
    uint width = src.cols;
    uint height = src.rows;
    int x, y;
    const auto filledLimit = (uint) round(height * filledRatioLimit / 2);

    // Scan first line to detect dominant color
    uint whitePixels = 0;
    uint blackPixels = 0;

    for (y = top; y < bottom; y+=2) {
        if (isBlackPixel(pixels, width, 0, y)) {
            blackPixels++;
        } else if (isWhitePixel(pixels, width, 0, y)) {
            whitePixels++;
        }
    }

    auto detectFunc = isBlackPixel;
    if (whitePixels > filledLimit && blackPixels > filledLimit) {
        // Mixed fill found... don't crop anything
        return 0;
    } else if (blackPixels > filledLimit) {
        detectFunc = isWhitePixel;
    }

    // Scan vertical lines in search of filled lines
    for (x = 1; x < width; x++) {
        uint filledCount = 0;

        for (y = top; y < bottom; y+=2) {
            if (detectFunc(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return x;
        }
    }

    // No fill found... don't crop anything
    return 0;
}

/** Return the first x position where there is a substantial amount of fill,
 * starting the search from the right. */
uint findBorderRight(const Mat& src, uint top, uint bottom) {
    uchar* pixels = src.data;
    uint width = src.cols;
    uint height = src.rows;
    int x, y;
    const auto filledLimit = (uint) round(height * filledRatioLimit / 2);

    // Scan first line to detect dominant color
    uint whitePixels = 0;
    uint blackPixels = 0;

    uint lastX = width - 1;
    for (y = top; y < bottom; y+=2) {
        if (isBlackPixel(pixels, width, lastX, y)) {
            blackPixels++;
        } else if (isWhitePixel(pixels, width, lastX, y)) {
            whitePixels++;
        }
    }

    auto detectFunc = isBlackPixel;
    if (whitePixels > filledLimit && blackPixels > filledLimit) {
        // Mixed fill found... don't crop anything
        return width;
    } else if (blackPixels > filledLimit) {
        detectFunc = isWhitePixel;
    }

    // Scan vertical lines in search of filled lines
    for (x = width - 2; x > 0; x--) {
        uint filledCount = 0;

        for (y = top; y < bottom; y+=2) {
            if (detectFunc(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return x + 1;
        }
    }

    // No fill found... don't crop anything
    return width;
}

/** Return the first y position where there is a substantial amount of fill,
 * starting the search from the top. */
uint findBorderTop(const Mat& src) {
    uchar* pixels = src.data;
    uint width = src.cols;
    uint height = src.rows;
    int x, y;
    const auto filledLimit = (uint) round(width * filledRatioLimit / 2);

    // Scan first line to detect dominant color
    uint whitePixels = 0;
    uint blackPixels = 0;

    for (x = 0; x < width; x+=2) {
        if (isBlackPixel(pixels, width, x, 0)) {
            blackPixels++;
        } else if (isWhitePixel(pixels, width, x, 0)) {
            whitePixels++;
        }
    }

    auto detectFunc = isBlackPixel;
    if (whitePixels > filledLimit && blackPixels > filledLimit) {
        // Mixed fill found... don't crop anything
        return 0;
    } else if (blackPixels > filledLimit) {
        detectFunc = isWhitePixel;
    }

    // Scan horizontal lines in search of filled lines
    for (y = 1; y < height; y++) {
        uint filledCount = 0;

        for (x = 0; x < width; x+=2) {
            if (detectFunc(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return y;
        }
    }

    // No fill found... don't crop anything
    return 0;
}

/** Return the first y position where there is a substantial amount of fill,
 * starting the search from the bottom. */
uint findBorderBottom(const Mat& src) {
    uchar* pixels = src.data;
    uint width = src.cols;
    uint height = src.rows;
    int x, y;
    const auto filledLimit = (uint) round(width * filledRatioLimit / 2);

    // Scan first line to detect dominant color
    uint whitePixels = 0;
    uint blackPixels = 0;
    uint lastY = height - 1;

    for (x = 0; x < width; x+=2) {
        if (isBlackPixel(pixels, width, x, lastY)) {
            blackPixels++;
        } else if (isWhitePixel(pixels, width, x, lastY)) {
            whitePixels++;
        }
    }

    auto detectFunc = isBlackPixel;
    if (whitePixels > filledLimit && blackPixels > filledLimit) {
        // Mixed fill found... don't crop anything
        return height;
    } else if (blackPixels > filledLimit) {
        detectFunc = isWhitePixel;
    }

    // Scan horizontal lines in search of filled lines
    for (y = height - 2; y > 0; y--) {
        uint filledCount = 0;

        for (x = 0; x < width; x+=2) {
            if (detectFunc(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return y + 1;
        }
    }

    // No fill found... don't crop anything
    return height;
}

Rect findBorders(const Mat& src) {
    uint top = findBorderTop(src);
    uint bottom = findBorderBottom(src);
    uint left = findBorderLeft(src, top, bottom);
    uint right = findBorderRight(src, top, bottom);

    return cv::Rect(left, top, right-left, bottom-top);
}
