#include <iostream>
#include <vector>
#include <valarray>

#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define KERNEL_SIZE 25
#define DISPARITY_RANGE 40
#define ADAPTIVE_THRESHOLD 32
#define MATCHING_THRESHOLD 8

typedef uint uint;

// float get_euclidean_distance(int a, int b);
float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2);

int main(int arc, char** argv)
{
	clock_t begin = clock();

	// setup
	cv::Mat image1 = cv::imread(argv[1], 1); // 0 flag loads as grayscale
	cvtColor(image1, image1, CV_BGR2Lab, 1);
	//cv::resize(image1, image1, cv::Size(), 0.5f, 0.5f, CV_INTER_LINEAR);

	cv::Mat image2 = cv::imread(argv[2], 1);
	cvtColor(image2, image2, CV_BGR2Lab);
	//cv::resize(image2, image2, cv::Size(), 0.5f, 0.5f, CV_INTER_LINEAR);
	
	const uint width = image1.cols;
	const uint height = image1.rows;

	cv::Mat output = image1.clone();
	output = cv::Scalar(0);

	// int pValue, qValue;
	int Lp, ap, bp, Lq, aq,bq;
	float difference;

	// for each pixel inside the matching region
	for (uint row = (KERNEL_SIZE >> 1) + 1; row < height - (KERNEL_SIZE >> 1) + 1; ++row)
	{
		for (uint col = (KERNEL_SIZE >> 1) + DISPARITY_RANGE; col < width - (KERNEL_SIZE >> 1) + 1; ++col)
		{
			Lp = image1.at<cv::Vec3b>(row, col).val[0];
			ap = image1.at<cv::Vec3b>(row, col).val[1];
			bp = image1.at<cv::Vec3b>(row, col).val[2];
			
			std::vector<uint> positivesY, positivesX; // containers for adaptive pixel coordinates

			// for each pixel of bucket
			for (int m = -(KERNEL_SIZE >> 1); m < KERNEL_SIZE >> 1; ++m) // ++m
			{
				for (int n = -(KERNEL_SIZE >> 1); n < KERNEL_SIZE >> 1; ++n) // ++n
				{
					// qValue = image1.at<uchar>(row + m, col + n);
					Lq = image1.at<cv::Vec3b>(row + m, col + n).val[0];
					aq = image1.at<cv::Vec3b>(row + m, col + n).val[1];
					bq = image1.at<cv::Vec3b>(row + m, col + n).val[2];

					// difference = get_euclidean_distance(pValue, qValue);
					difference = get_euclidean_distance(Lp, ap, bp, Lq, aq, bq);
					if (difference < ADAPTIVE_THRESHOLD)
					{
						positivesY.push_back(row + m);
						positivesX.push_back(col + n);
					}
				}
			}

			std::valarray<uint> C; // number of matches for given disparity
			C.resize(DISPARITY_RANGE + 1, 0);

			uint numberOfPositives = static_cast<uint>(positivesX.size());

			// TODO: bezveze
			int activeIndex = -1;

			// matching
			for (int disparity = -DISPARITY_RANGE; disparity < 0; ++disparity)
			{
				++activeIndex;
				for (uint pixel = 0; pixel < numberOfPositives; ++pixel)
				{
					// pValue = image1.at<uchar>(positivesY[pixel], positivesX[pixel]);
					// qValue = image2.at<uchar>(positivesY[pixel], positivesX[pixel] + disparity);
					Lp = image1.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel]).val[0];
					ap = image1.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel]).val[1];
					bp = image1.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel]).val[2];
					Lq = image2.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel] + disparity).val[0];
					aq = image2.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel] + disparity).val[1];
					bq = image2.at<cv::Vec3b>(positivesY[pixel], positivesX[pixel] + disparity).val[2];
					// difference = get_euclidean_distance(pValue, qValue);
					difference = get_euclidean_distance(Lp, ap, bp, Lq, aq, bq);
					if (difference < MATCHING_THRESHOLD)
					{
						C[activeIndex] = C[activeIndex] + 1;
					}
				}
			}

			// TODO: remove maximum ambiguity
			uint maximum = C.max();

			// TODO: find index of maximum (avoid for loop)
			for (uint index = 0; index < C.size(); ++index)
			{
				if (C[index] == maximum)
				{
					// output.at<cv::Vec3b>(row, col) = (DISPARITY_RANGE - index) * 255 / (DISPARITY_RANGE + 1); }
					output.at<cv::Vec3b>(row, col).val[0] = (DISPARITY_RANGE - index) * 255 / (DISPARITY_RANGE + 1);
					output.at<cv::Vec3b>(row, col).val[1] = (DISPARITY_RANGE - index) * 255 / (DISPARITY_RANGE + 1);
					output.at<cv::Vec3b>(row, col).val[2] = (DISPARITY_RANGE - index) * 255 / (DISPARITY_RANGE + 1);
					break;
				}
			}
		}
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	std::cout << time_spent << std::endl;

	// cv::resize(output, output, cv::Size(), 2.0f, 2.0f, CV_INTER_NN);
	cv::imshow("Output", output);
	cv::waitKey();
	return 0;
}

/*float get_euclidean_distance(int a, int b)
{
	return static_cast<float>(abs(a - b));
}*/

float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2)
{	
	return abs(a1 - a2) + abs(b1 - b2) + abs(c1 - c2);
}

/*float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2)
{
	float d;

	int da = a1 - a2;
	int db = b1 - b2;
	int dc = c1 - c2;

	d = da * da + db * db + dc * dc;
	
	return sqrt(d);
}*/
