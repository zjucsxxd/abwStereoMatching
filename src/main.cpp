#include "stl.h"
#include "cv.h"
#include "cvInterface.h"
#include "valarray2d.h"
#include "bucket.h"

// #define WRITE_OUTPUT;

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

	Mat image_copy{ image1.clone() };

	// parameters
	const double threshold{ 8.0 };
	const unsigned int bucketSize{ 16 };

	// for each pixel of image1
	for (unsigned int row{ 200 }; row < 550; row++) // row += bucketSize
	{
		for (unsigned int col{ 650 }; col < 1000; col++) // col += bucketSize
		{
			// determine ABW shape

			// construct ABW bucket

			bucket <double> testWindow{ row, col, bucketSize };

			// crop bucket

			if (testWindow.get_x1() < 0) { testWindow.set_x1(0); }
			if (testWindow.get_y1() < 0) { testWindow.set_y1(0); }
			if (testWindow.get_x2() > image1.cols) { testWindow.set_x2(image1.cols); }
			if (testWindow.get_y2() > image1.rows) { testWindow.set_y2(image1.rows); }

			int leftBorder{ testWindow.get_x1() };
			int topBorder{ testWindow.get_y1() };
			int rightBorder{ testWindow.get_x2() };
			int bottomBorder{ testWindow.get_y2() };

			int rl{ leftBorder }; // temporary
			int rt{ topBorder }; // temporary

			// evaluate center bucket pixel

			double Lp{};
			double ap{};
			double bp{};
			readLabPixel(image1_lab, Lp, ap, bp, row, col);

			// prepare variables for other bucket pixels

			double Lq{};
			double aq{};
			double bq{};

			double dpq{};

			// for each pixel of bucket
			std::vector<int> positivesY{};
			std::vector<int> positivesX{};

			while (leftBorder < rightBorder)
			{
				topBorder = rt;
				while (topBorder < bottomBorder)
				{
					readLabPixel(image1_lab, Lq, aq, bq, topBorder, leftBorder);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < threshold)
					{
						testWindow.set_value(leftBorder - rl, topBorder - rt, 255);
						positivesY.push_back(topBorder); // Y-coordinate of image1_lab active matching pixel
						positivesX.push_back(leftBorder); // X-coordinate of image1_lab active matching pixel
					}
					topBorder++;
				}
				leftBorder++;
			}

			// block matching distances
			const int maxDisparity{ 20 };

			std::valarray<int> count{};
			count.resize(maxDisparity * 2 + 1, 0);

			int activeCount{ 0 };

			int loopSize = positivesX.size();

			int T{ 64 };

			for (int disparities{ -maxDisparity }; disparities < maxDisparity; disparities++)
			{
				for (int activePixels{ 0 }; activePixels < loopSize; activePixels++)
				{
					readLabPixel(image1_lab, Lp, ap, bp, positivesY[activePixels], positivesX[activePixels]);
					readLabPixel(image2_lab, Lq, aq, bq, positivesY[activePixels] + disparities, positivesX[activePixels] + disparities);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < T)
					{
						count[activeCount] = count[activeCount] + 1;
					}
				}
				activeCount++;
			}
			int maximum{ count.max() };
			for (int counts{ 0 }; counts < count.size(); counts++)
			{
				if (count[counts] == maximum)
				{
					image_copy.at<Vec3b>(row, col).val[0] = counts * 255 / 40;
					image_copy.at<Vec3b>(row, col).val[1] = counts * 255 / 40;
					image_copy.at<Vec3b>(row, col).val[2] = counts * 255 / 40;
					break;
				}
			}
		}
	}

	//imshow("Output", image_copy);
	//waitKey(1000);
	//stringstream filename{};
	//filename << "image1_abw_" << j << "_" << i << "_size" << bucketSize << "_t" << threshold << ".png";
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