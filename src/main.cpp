#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "stereo.hpp"
#include "utilities.hpp"

int main(int arc, char** argv)
{
	cv::Mat left  = cv::imread(argv[1], 1);
	cv::Mat right = cv::imread(argv[2], 1);

	const int kernelSize = static_cast<int>(*argv[3]); // not being used atm

	clock_t begin = clock();
	cv::Mat disparity = generate_disparity_map(left, right);
	clock_t end = clock();

	echo_interval(begin, end);

	cv::imshow("Disparity", disparity);
	cv::waitKey();

	return 0;
}
