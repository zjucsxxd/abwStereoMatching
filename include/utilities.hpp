#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

inline int taxicab_dist(const uchar a[3], const uchar b[3]) { return (abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2])); }

void preprocess(cv::Mat& image);
void postprocess(cv::Mat& image);

#endif
