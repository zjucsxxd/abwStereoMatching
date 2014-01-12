#include "stl.h"
#include "cv.h"
#include "cvInterface.h"
#include "valarray2d.h"
#include "ABW.h"

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

	// CIELab conversion
	Mat image1_lab{};
	Mat image2_lab{};
	cvtColor(image1, image1_lab, CV_RGB2Lab);
	cvtColor(image2, image2_lab, CV_RGB2Lab);

	// parameters
	const double threshold{ 8.0 };

	// bucketSize must be even
#ifdef DRAW_OUTPUT
	const unsigned int bucketSize{ 256 };
#else
	const unsigned int bucketSize{ 16 };
#endif

	// for each pixel of image1
#ifdef DRAW_OUTPUT
	for (int i{ bucketSize / 2 }; i < height1; i += bucketSize)
	{
		for (int j{ bucketSize / 2 }; j < width1; j += bucketSize)
		{
#else
	for (int i{ 0 }; i < 480; i++)
	{
		for (int j{ 0 }; j < 640; j++)
		{
#endif
			// bucket construction
			ABW <double> testWindow{ j, i, bucketSize };

			double Lp{};
			double ap{};
			double bp{};

			// ABW shape determination
			readLabPixel(image1_lab, Lp, ap, bp, i, j);

			int x1{ testWindow.get_x1() };
			int y1{ testWindow.get_y1() };
			int x2{ testWindow.get_x2() };
			int y2{ testWindow.get_y2() };

			if (x1 < 0) { x1 = 0; }
			if (y1 < 0) { y1 = 0; }
			if (x2 > width1) { x2 = width1; }
			if (y2 > height1) { y2 = height1; }

			double Lq{};
			double aq{};
			double bq{};

			double dpq{};

			// for each pixel of bucket
#ifdef DRAW_OUTPUT
			Mat image1_copy{ image1.clone() };
#endif
			for (int k{ x1 }; k < x2; k++)
			{
				for (int l{ y1 }; l < y2; l++)
				{
					readLabPixel(image1_lab, Lq, aq, bq, l, k);
					distance(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < threshold) { testWindow.set_value(k - x1, l - y1, 255); }
#ifdef DRAW_OUTPUT
					image1_copy.at<Vec3b>(l, k).val[0] = testWindow.get_value(k - x1, l - y1);
					image1_copy.at<Vec3b>(l, k).val[1] = testWindow.get_value(k - x1, l - y1);
					image1_copy.at<Vec3b>(l, k).val[2] = testWindow.get_value(k - x1, l - y1);
#endif
				}
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