#ifndef STEREO_HPP_
#define STEREO_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat generate_disparity_map(cv::Mat image1, cv::Mat image2);
uchar get_pixel_disparity(cv::Mat& image1, cv::Mat& image2, cv::Mat::MStep step, int row, int col);
void get_abw_coords(cv::Mat& image1, int row, int col, const uchar pValues[3], uchar qValues[3], std::vector<uint>& positivesX, std::vector<uint>& positivesY);
void match(cv::Mat& image1, cv::Mat& image2, std::vector<uint>& Y, std::vector<uint>& X, std::vector<uint>& C);

#endif
