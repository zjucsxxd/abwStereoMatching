#ifndef STEREO_HPP_
#define STEREO_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct points
{
	std::vector<unsigned int> x;
	std::vector<unsigned int> y;
};

void process_stereo_input (cv::Mat& image);
void process_stereo_output(cv::Mat& image);

cv::Mat generate_disparity_map(cv::Mat& image1, cv::Mat& image2);
uchar get_pixel_disparity(cv::Mat& image1, cv::Mat& image2, int row, int col);
points get_abw_coords(cv::Mat& image, int row, int col);
std::vector<int> match(cv::Mat& image1, cv::Mat& image2, points&);

#endif
