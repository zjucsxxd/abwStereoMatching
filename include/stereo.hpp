#ifndef STEREO_HPP_
#define STEREO_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void preprocess(cv::Mat& image);
void postprocess(cv::Mat& image);
cv::Mat generate_disparity_map(cv::Mat& image1, cv::Mat& image2);
uchar get_pixel_disparity(uchar* data1, uchar* data2, cv::Mat::MStep step, int row, int col, const int& kernelSize);

#endif
