#include "stereo.hpp"

#include <valarray>

#define DISPARITY_RANGE 40
#define ADAPTIVE_THRESHOLD 32
#define MATCHING_THRESHOLD 8

typedef unsigned int uint;

cv::Mat generate_disparity_map(cv::Mat& image1, cv::Mat& image2)
{
	int kernelSize = 25; // TODO: adaptive kernel size

	preprocess(image1);
	preprocess(image2);

	cv::Mat output = image1.clone();
	output = cv::Scalar(0);

	const uint width = image1.cols;
	const uint height = image1.rows;

	// int pValue, qValue;
	int Lp, ap, bp, Lq, aq,bq;
	float difference;

	// for each pixel inside the matching region
	for (uint row = (kernelSize / 2) + 1; row < height - (kernelSize / 2) + 1; ++row)
	{
		for (uint col = (kernelSize / 2) + DISPARITY_RANGE; col < width - (kernelSize / 2) + 1; ++col)
		{
			Lp = image1.at<cv::Vec3b>(row, col).val[0];
			ap = image1.at<cv::Vec3b>(row, col).val[1];
			bp = image1.at<cv::Vec3b>(row, col).val[2];

			std::vector<uint> positivesY, positivesX; // containers for adaptive pixel coordinates

			// for each pixel of bucket
			for (int m = -(kernelSize / 2); m < kernelSize / 2; ++m) // ++m
			{
				for (int n = -(kernelSize / 2); n < kernelSize / 2; ++n) // ++n
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

	postprocess(output);

	return output;
}

float get_euclidean_distance(int a1, int b1, int c1, int a2, int b2, int c2)
{
	return static_cast<float>(abs(a1 - a2) + abs(b1 - b2) + abs(c1 - c2));
}

void preprocess(cv::Mat& image)
{
	cvtColor(image, image, CV_BGR2Lab, 1);
	cv::resize(image, image, cv::Size(), 0.5f, 0.5f, CV_INTER_LINEAR);
}

void postprocess(cv::Mat& image)
{
	cv::resize(image, image, cv::Size(), 2.0f, 2.0f, CV_INTER_NN);
}
