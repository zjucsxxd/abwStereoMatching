#include <iostream>
#include <vector>

#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "stereo.hpp"

void echo_interval(clock_t t1, clock_t t2);

int main(int arc, char** argv)
{
	cv::Mat left  = cv::imread(argv[1], 1);
	cv::Mat right = cv::imread(argv[2], 1);

	clock_t begin = clock();
	cv::Mat disparity = generate_disparity_map(left, right);
	clock_t end = clock();

	echo_interval(begin, end);

	cv::imshow("Input", left);
	cv::imshow("Output", disparity);
	cv::waitKey();

	return 0;
}

void echo_interval(clock_t t1, clock_t t2)
{
	double interval = (double)(t2 - t1) / CLOCKS_PER_SEC;
	std::cout << interval << std::endl;
}
