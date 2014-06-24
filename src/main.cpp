#include <iostream>
#include <vector>

#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "stereo.hpp"

// float get_euclidean_distance(int a, int b);

int main(int arc, char** argv)
{
	clock_t begin = clock();

	cv::Mat left  = cv::imread(argv[1], 1);
	cv::Mat right = cv::imread(argv[2], 1);

	cv::Mat disparity = generate_disparity_map(left, right);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	std::cout << time_spent << std::endl;
	cv::imshow("Input", left);
	cv::imshow("Output", disparity);
	cv::waitKey();
	return 0;
}

/*float get_euclidean_distance(int a, int b)
{
return static_cast<float>(abs(a - b));
}*/



/*float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2)
{
float d;

int da = a1 - a2;
int db = b1 - b2;
int dc = c1 - c2;

d = da * da + db * db + dc * dc;

return sqrt(d);
}*/
