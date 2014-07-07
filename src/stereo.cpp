#include "stereo.hpp"
#include "utilities.hpp"

typedef unsigned int uint;

// TODO: +control, -global
const int kernelSize = 25;
const int disparityRange = 20;
const int adaptiveThreshold = 32;
const int matchingThreshold = 8;

void process_stereo_input(cv::Mat& image)
{
	while(!image.isContinuous()) { image = image.clone(); }
	cv::resize(image, image, cv::Size(), 0.5f, 0.5f, CV_INTER_LINEAR);
	cv::cvtColor(image, image, CV_BGR2Lab, 1);
}

void process_stereo_output(cv::Mat& image)
{
	cv::resize(image, image, cv::Size(), 2.0f, 2.0f, CV_INTER_NN);
}

cv::Mat generate_disparity_map(cv::Mat& image1, cv::Mat& image2)
{
	process_stereo_input(image1);
	process_stereo_input(image2);

	const int height = image1.rows;
	const int width  = image1.cols;

	cv::Mat output(height, width, CV_8UC1);
	output = cv::Scalar(0);

	// for each pixel inside the matching region
	const int border = kernelSize / 2 + 1;
	uchar* data;
	for (int row = border; row < height - border; ++row)
	{
		data = output.ptr<uchar>(row) + border + disparityRange;
		for (int col = border + disparityRange; col < width - border; ++col)
		{
			*data++ = get_pixel_disparity(image1, image2, row, col);
		}
	}

	process_stereo_output(output);

	return output;
}

uchar get_pixel_disparity(cv::Mat& image1, cv::Mat& image2, int row, int col)
{
	points abwCoordinates = get_abw_coords(image1, row, col);

	// number of matches for given disparity
	std::vector<int> C = match(image1, image2, abwCoordinates);

	// TODO: make more elegant
	int maximum = *std::max_element(C.begin(), C.end());
	int ambiguous = 0;
	for (int a = 0; a < C.size(); ++a)
	{
		if (C[a] == maximum) { ambiguous += 1; }
	}

	// TODO: find index of maximum (avoid for loop)
	for (int index = 0; index < C.size(); ++index)
	{
		if (C[index] == maximum && ambiguous == 1)
		{
			return (disparityRange - index) * 255 / disparityRange;
			break;
		}
	}
}

points get_abw_coords(cv::Mat& image, int row, int col)
{
	uchar center[3];
	uchar  other[3];
	points positives;

	// TODO: determine average number of abw pixels
	const int reserveApprox = kernelSize * kernelSize / 2;
	positives.x.reserve(reserveApprox);
	positives.y.reserve(reserveApprox);

	uchar* data;
	data = image.ptr<uchar>(row) + (col * image.channels());

	center[0] = *data++;
	center[1] = *data++;
	center[2] = *data++;

	// for each pixel of bucket
	for (int m = -(kernelSize / 2); m < kernelSize / 2; ++m)
	{
		data = image.ptr<uchar>(row + m) + (col - kernelSize / 2)  * 3;
		for (int n = -(kernelSize / 2); n < kernelSize / 2; ++n)
		{
			other[0] = *data++;
			other[1] = *data++;
			other[2] = *data++;

			if (taxicab_dist(center, other) < adaptiveThreshold)
			{
				positives.y.push_back(row + m);
				positives.x.push_back(col + n);
			}
		}
	}

	return positives;
}

std::vector<int> match(cv::Mat& image1, cv::Mat& image2, points& coords)
{
	std::vector<int> C(disparityRange, 0);
	int numberOfPositives = coords.x.size();

	uchar* data1;
	uchar* data2;
	uchar rVals[3];
	uchar lVals[3];

	// matching ** MEMORY ACCESS BOTTLENECK**
	for (int pixel = 0; pixel < numberOfPositives; ++pixel)
	{
		data1 = image1.ptr<uchar>(coords.y[pixel]) +  coords.x[pixel]                   * 3;
		data2 = image2.ptr<uchar>(coords.y[pixel]) + (coords.x[pixel] - disparityRange) * 3;

		rVals[0] = *data1++;
		rVals[1] = *data1++;
		rVals[2] = *data1++;

		for (int disparity = 0; disparity < disparityRange; ++disparity)
		{
			lVals[0] = *data2++;
			lVals[1] = *data2++;
			lVals[2] = *data2++;

			if (taxicab_dist(rVals, lVals) < matchingThreshold)
			{
				++C[disparity];
			}
		}
	}

	return C;
}
