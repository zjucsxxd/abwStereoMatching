#ifndef STEREO_HPP_
#define STEREO_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat generate_disparity_map(cv::Mat image1, cv::Mat image2);
void preprocess(cv::Mat& image);
void postprocess(cv::Mat& image);
float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2);
float get_euclidean_distance(uchar a, uchar b);

#endif
