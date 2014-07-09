#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "stereo.hpp"
#include "utilities.hpp"

int main(int arc, char** argv)
{
	const int kernelSize = static_cast<int>(*argv[3]); // not being used atm

	if(static_cast<int>(*argv[4]) == 0) // maybe not needed, since cv::VideoCapture can read images as well
	{
		cv::Mat left  = cv::imread(argv[1], 1);
		cv::Mat right = cv::imread(argv[2], 1);

		clock_t begin = clock();
		cv::Mat disparity = generate_disparity_map(left, right);
		clock_t end = clock();

		echo_interval(begin, end);

		cv::imshow("Disparity", disparity);
		cv::waitKey();
	}
	else
	{
		cv::Size frameSize(640, 480); // hard-coded for my specific camera atm
		int frame = 0;
		cv::VideoCapture lStream(argv[1]);
		cv::VideoCapture rStream(argv[2]);
		cv::VideoWriter oStream ("output.avi", 0, 24, frameSize, false);
		if(!(lStream.isOpened() && rStream.isOpened()))
			{
				std::cerr << "OOPS";
				return -1;
			}
		cv::Mat left, right;
		while(true)
		{
			lStream >> left;
			rStream >> right;
			if(left.empty() || right.empty()) { break; }
			oStream.write(generate_disparity_map(left, right));
			++frame;
			std::cout << frame << std::endl; // basic progress tracking
		}
	}

	return 0;
}
