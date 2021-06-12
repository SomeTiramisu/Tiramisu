// Adapted from https://github.com/tachiyomiorg/image-decoder
//
// Created by len on 25/12/20.
//

#ifndef CROP_H
#define CROP_H

#include <stdlib.h>
#include <opencv2/core.hpp>

/** A line will be considered as having content if 0.25% of it is filled. */
const float filledRatioLimit = 0.0025;

/** When the threshold is closer to 1, less content will be cropped. **/
#define THRESHOLD 0.75

const uchar thresholdForBlack = (uchar)(255.0 * THRESHOLD);

const uchar thresholdForWhite = (uchar)(255.0 - 255.0 * THRESHOLD);

/** Finds the borders of the image. This only works on bitmaps of a single component (grayscale) **/
cv::Rect findBorders(const cv::Mat& src);

#endif //CROP_H
