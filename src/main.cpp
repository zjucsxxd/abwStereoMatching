#include "stl.h"
#include "cv.h"
#include "cvInterface.h"
#include "valarray2d.h"
#include "bucket.h"

#define DRAW_OUTPUT
// #define WRITE_OUTPUT

void distance(double& dpq, double& Lp, double& Lq, double& ap, double& aq, double& bp, double& bq);

int main(int argc, char** argv)
{

	// input
	Mat image1{ imread(argv[1]) };
	Mat image2{ imread(argv[2]) };
	const unsigned int width1{ static_cast<unsigned int>(image1.cols) };
	const unsigned int height1{ static_cast<unsigned int>(image1.rows) };
	const unsigned int width2{ static_cast<unsigned int>(image2.cols) };
	const unsigned int height2{ static_cast<unsigned int>(image2.rows) };

	// convert to CIELab
	Mat image1_lab{};
	Mat image2_lab{};
	cvtColor(image1, image1_lab, CV_RGB2Lab);
	cvtColor(image2, image2_lab, CV_RGB2Lab);

	// parameters
	const double threshold{ 8.0 };
#ifdef DRAW_OUTPUT
	const unsigned int bucketSize{ 256 };
#else
	const unsigned int bucketSize{ 16 };
#endif

	// for each pixel of image1
#ifdef DRAW_OUTPUT
	for (unsigned int row{ bucketSize / 2 }; row < height1; row += bucketSize)
	{
		for (unsigned int col{ bucketSize / 2 }; col < width1; col += bucketSize)
		{
#else
	for (unsigned int row{ 0 }; row < 480; row++)
	{
		for (unsigned int col{ 0 }; col < 640; col++)
		{
#endif
			// determine ABW shape
			
			// construct ABW bucket
			bucket <double> testWindow{ row, col, bucketSize };

			// crop bucket
			if (testWindow.get_x1() < 0) { testWindow.set_x1(0); }
			if (testWindow.get_y1() < 0) { testWindow.set_y1(0); }
			if (testWindow.get_x2() > width1) { testWindow.set_x2(width1); }
			if (testWindow.get_y2() > height1) { testWindow.set_y2(height1); }

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
#ifdef DRAW_OUTPUT
			Mat image1_copy{ image1.clone() };
#endif
			while (leftBorder < rightBorder)
			{
				topBorder = rt;
				while (topBorder < bottomBorder)
				{
					readLabPixel(image1_lab, Lq, aq, bq, topBorder, leftBorder);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < threshold) { testWindow.set_value(leftBorder - rl, topBorder - rt, 255); }
#ifdef DRAW_OUTPUT
					image1_copy.at<Vec3b>(topBorder, leftBorder).val[0] = testWindow.get_value(leftBorder - rl, topBorder - rt);
					image1_copy.at<Vec3b>(topBorder, leftBorder).val[1] = testWindow.get_value(leftBorder - rl, topBorder - rt);
					image1_copy.at<Vec3b>(topBorder, leftBorder).val[2] = testWindow.get_value(leftBorder - rl, topBorder - rt);
#endif
					topBorder++;
				}
				leftBorder++;
			}
#ifdef DRAW_OUTPUT
			imshow("Output", image1_copy);
			waitKey(1000);
#endif
#ifdef WRITE_OUTPUT
			stringstream filename{};
			filename << "image1_abw_" << j << "_" << i << "_size" << bucketSize << "_t" << threshold << ".png";
			imwrite(filename.str().c_str(), image1_copy);
#endif
		}
	}

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