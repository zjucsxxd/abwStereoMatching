#include "utilities.hpp"

void preprocess(cv::Mat& image)
{
	cv::resize(image, image, cv::Size(), 0.5f, 0.5f, CV_INTER_LINEAR);
	cvtColor(image, image, CV_BGR2Lab, 1);
}

void postprocess(cv::Mat& image)
{
	cv::resize(image, image, cv::Size(), 2.0f, 2.0f, CV_INTER_NN);
}
