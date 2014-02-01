#include "stl.h"
#include "cv.h"
#include "cvInterface.h"
#include "valarray2d.h"
#include "bucket.h"

void distance(double& dpq, double& Lp, double& Lq, double& ap, double& aq, double& bp, double& bq);

int main(int argc, char** argv)
{

	// input
	Mat image1{ imread(argv[1]) };
	Mat image2{ imread(argv[2]) };
	const unsigned int width{ static_cast<unsigned int>(image1.cols) };
	const unsigned int height{ static_cast<unsigned int>(image1.rows) };

	// convert to CIELab
	Mat image1_lab{};
	Mat image2_lab{};
	cvtColor(image1, image1_lab, CV_RGB2Lab);
	cvtColor(image2, image2_lab, CV_RGB2Lab);

	// create what will be shown in the "Output" window
	Mat image_copy{ image1.clone() };

	// parameters
	const double adaptiveThreshold{ 8.0 };
	const int bucketSize{ 16 }; // use even positive value
	const int disparityRange{ 20 };

	// for each pixel of image1
	for (unsigned int row{ bucketSize / 2 }; row < height - bucketSize / 2; row++)
	{
		for (unsigned int col{ bucketSize / 2 + disparityRange }; col < width - bucketSize / 2; col++)
		{
			// determine ABW shape

			// construct ABW bucket
			// bucket <double> activeMatchingWindow{ row, col, bucketSize };

			// crop bucket

			// if (activeMatchingWindow.get_x1() < 0) { activeMatchingWindow.set_x1(0); }
			// if (activeMatchingWindow.get_y1() < 0) { activeMatchingWindow.set_y1(0); }
			// if (activeMatchingWindow.get_y2() > height) { activeMatchingWindow.set_y2(height); }
			// if (activeMatchingWindow.get_x2() > width) { activeMatchingWindow.set_x2(width); }

			// int topBorder{ activeMatchingWindow.get_y1() }; 
			// int leftBorder{ activeMatchingWindow.get_x1() };
			// int bottomBorder{ activeMatchingWindow.get_y2() };
			// int rightBorder{ activeMatchingWindow.get_x2() };

			// store initial border coordinates
			// int rememberLeft{ leftBorder };
			// int rememberTop{ topBorder };

			// evaluate center bucket pixel Lab values
			double Lp{};
			double ap{};
			double bp{};
			readLabPixel(image1_lab, Lp, ap, bp, row, col);

			// prepare variables for other bucket pixels
			double Lq{};
			double aq{};
			double bq{};

			double dpq{};

			// store matched pixel coordinates
			std::vector<unsigned int> positivesY{};
			std::vector<unsigned int> positivesX{};

			// for each pixel of bucket
			for (int m = -bucketSize / 2; m < bucketSize / 2; m++)
			{
				for (int n{ -bucketSize / 2 }; n < bucketSize / 2; n++)
				{
					readLabPixel(image1_lab, Lq, aq, bq, row + m, col + n);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < adaptiveThreshold)
					{
						positivesY.push_back(row + m);
						positivesX.push_back(col + n);
					}
				}
			}

			// block matching

			// parameters

			const double matchThreshold{ 64.0 };
			std::valarray<int> count{}; // store number of matches for given disparity
			count.resize(disparityRange + 1, 0);

			int positives = positivesX.size(); // number of positives for given window

			int activeIndex{ -1 };

			for (int currentDisparity{ -disparityRange }; currentDisparity <= 0; currentDisparity++)
			{
				activeIndex++;
				for (int activePixels{ 0 }; activePixels < positives; activePixels++)
				{
					readLabPixel(image1_lab, Lp, ap, bp, positivesY[activePixels], positivesX[activePixels]);
					readLabPixel(image2_lab, Lq, aq, bq, positivesY[activePixels], positivesX[activePixels] + currentDisparity);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < matchThreshold)
					{
						count[activeIndex] = count[activeIndex] + 1;
					}
				}
			}

			// TODO: determine if maximum is unique (ambiguity)
			int maximum{ count.max() };

			// TODO: find index of maximum (avoid for loop)
			for (int index{ 0 }; index < count.size(); index++)
			{
				if (count[index] == maximum)
				{
					image_copy.at<Vec3b>(row, col).val[0] = index * 255 / (disparityRange + 1);
					image_copy.at<Vec3b>(row, col).val[1] = index * 255 / (disparityRange + 1);
					image_copy.at<Vec3b>(row, col).val[2] = index * 255 / (disparityRange + 1);
					break;
				}
			}
		}
	}

	imwrite("disp.png", image_copy);

	return 0;
}

void distance(double& dpq, double& Lp, double& Lq, double& ap, double& aq, double& bp, double& bq)
{
	dpq = 0;
	dpq += pow((Lp - Lq), 2);
	dpq += pow((ap - aq), 2);
	dpq += pow((bp - bq), 2);
	dpq = sqrt(dpq);
}