#include "stereo.hpp"
#include "utilities.hpp"

typedef unsigned int uint;
typedef unsigned char uchar;

// TODO: +control, -global
const int kernelSize = 25;
const int disparityRange = 20;
const int adaptiveThreshold = 32;
const int matchingThreshold = 8;

cv::Mat generate_disparity_map(cv::Mat image1, cv::Mat image2)
{
	preprocess(image1);
	preprocess(image2);

	const uint height = static_cast<uint>(image1.rows);
	const uint width  = static_cast<uint>(image1.cols); // image1 is arbitrary

	cv::Mat output(height, width, CV_8UC1);
	output = cv::Scalar(0);

	uchar* data1 = image1.data;
	uchar* data2 = image2.data;
	cv::Mat::MStep step = image1.step;

	// for each pixel inside the matching region
	for (int row = (kernelSize / 2) + 1; row < height - (kernelSize / 2) + 1; ++row)
	{
		for (int col = (kernelSize / 2) + disparityRange; col < width - (kernelSize / 2) + 1; ++col)
		{
			output.at<uchar>(row, col) = get_pixel_disparity(image1, image2, step, row, col);
		}
	}

	postprocess(output);

	return output;
}

uchar get_pixel_disparity(cv::Mat& image1, cv::Mat& image2, cv::Mat::MStep step, int row, int col)
{
	uchar pValues[3];
	uchar qValues[3];

	// containers for adaptive pixel coordinates
	std::vector<uint> positivesY, positivesX;
	// TODO: determine average number of abw pixels
	int reserveApprox = kernelSize * kernelSize / 2;
	positivesY.reserve(reserveApprox);
	positivesX.reserve(reserveApprox);

	uchar* data1;
	uchar* data2;

	data1 = image1.ptr<uchar>(row) + (col * 3);
	pValues[0] = *data1++;
	pValues[1] = *data1++;
	pValues[2] = *data1++;

	get_abw_coords(image1, row, col, pValues, qValues, positivesX, positivesY);

	std::vector<uint> C(disparityRange, 0); // number of matches for given disparity

	match(image1, image2, positivesY, positivesX, C);

	// TODO: make more elegant
	uint maximum = *std::max_element(C.begin(), C.end());
	int ambiguous = 0;
	for (int a = 0; a < C.size(); ++a)
	{
		if (C[a] == maximum) { ambiguous += 1; }
	}

	// TODO: find index of maximum (avoid for loop)
	for (uint index = 0; index < C.size(); ++index)
	{
		if (C[index] == maximum && ambiguous == 1)
		{
			return (disparityRange - index) * 255 / disparityRange;
			break;
		}
	}
}

void get_abw_coords(cv::Mat& image1, int row, int col, const uchar pValues[3], uchar qValues[3], std::vector<uint>& positivesX, std::vector<uint>& positivesY)
{
	// for each pixel of bucket
	uchar* data1;
	for (int m = -(kernelSize / 2); m < kernelSize / 2; ++m) // ++m
	{
		data1 = image1.ptr<uchar>(row + m) + (col - kernelSize / 2)  * 3;
		for (int n = -(kernelSize / 2); n < kernelSize / 2; ++n) // ++n
		{
			qValues[0] = *data1++;
			qValues[1] = *data1++;
			qValues[2] = *data1++;

			if (taxicab_dist(pValues, qValues) < adaptiveThreshold)
			{
				positivesY.push_back(row + m);
				positivesX.push_back(col + n);
			}
		}
	}
}

void match(cv::Mat& image1, cv::Mat& image2, std::vector<uint>& Y, std::vector<uint>& X, std::vector<uint>& C)
{
	uint numberOfPositives = static_cast<uint>(X.size());
	uchar* data1;
	uchar* data2;
	uchar pValues[3];
	uchar qValues[3];

	// matching ** MEMORY ACCESS BOTTLENECK**
	for (uint pixel = 0; pixel < numberOfPositives; ++pixel)
	{
		data1 = image1.ptr<uchar>(Y[pixel]) + X[pixel] * 3;
		data2 = image2.ptr<uchar>(Y[pixel]) + (X[pixel] - disparityRange) * 3;

		pValues[0] = *data1++;
		pValues[1] = *data1++;
		pValues[2] = *data1++;
		for (int disparity = 0; disparity < disparityRange; ++disparity)
		{
			qValues[0] = *data2++;
			qValues[1] = *data2++;
			qValues[2] = *data2++;

			if (taxicab_dist(pValues, qValues) < matchingThreshold)
			{
				++C[disparity];
			}
		}
	}
}
