#include "stl.h"
#include "cv.h"
#include "matrix.h"
#include "ABW.h"
#include "matrixio.h"

#define DRAW_OVERLAY

void formula1(double& dpq, double Lp, double Lq, double ap, double aq, double bp, double bq);

int main(int argc, char** argv)
{

	/// input
	Mat image1{ imread(argv[1]) };
	Mat image2{ imread(argv[2]) };
	int width1{ image1.cols };
	int height1{ image1.rows };
	int width2{ image2.cols };
	int height2{ image2.rows };

	imwrite("image1.png", image1);

	/// CIELab conversion
	Mat image1_lab{};
	Mat image2_lab{};
	cvtColor(image1, image1_lab, CV_RGB2Lab);
	cvtColor(image2, image2_lab, CV_RGB2Lab);

	/// parameters
	double threshold{ 8.0 };
	int const bucketSize{ 512 };

	/// for each pixel of image1
	for (int i{ 0 }; i < image1.rows; i += bucketSize)
	{
		for (int j{ 0 }; j < image1.cols; j += bucketSize)
		{
			/// bucket construction
			ABW<double> testWindow{ j, i, bucketSize };

			double Lp{};
			double ap{};
			double bp{};

			/// ABW shape determination
			readLabPixel(image1_lab, Lp, ap, bp, i, j);

			int x1{ testWindow.get_x1() };
			int y1{ testWindow.get_y1() };
			int x2{ testWindow.get_x2() };
			int y2{ testWindow.get_y2() }; 

			if (x1 < 0) { x1 = 0; }
			if (y1 < 0) { y1 = 0; }
			if (x2 > width1) { x2 = width1; }
			if (y2 > height1) { y2 = height1; }
						
			//int* xq_ptr{ &xq };
			//int* yq_ptr{ &yq };

			double Lq{};
			double aq{};
			double bq{};

			double dpq{};

			/// for each pixel of bucket
#ifdef DRAW_OVERLAY
			Mat image1_copy{ image1.clone() };
#endif
			for (int k{ x1 }; k < x2; k++)
			{
				for (int l{ y1 }; l < y2; l++)
				{
					readLabPixel(image1_lab, Lq, aq, bq, l, k);
					formula1(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < threshold) { testWindow.set_value(k - x1, l - y1, 255); }
#ifdef DRAW_OVERLAY
					image1_copy.at<Vec3b>(l, k).val[0] = testWindow.get_value(k - x1, l - y1);
					image1_copy.at<Vec3b>(l, k).val[1] = testWindow.get_value(k - x1, l - y1);
					image1_copy.at<Vec3b>(l, k).val[2] = testWindow.get_value(k - x1, l - y1);
#endif
				}
			}
#ifdef DRAW_OVERLAY
			stringstream filename{};
			filename << "image1_abw_" << i << "_" << j << "_size" << bucketSize << "_t" << threshold << ".png";
			imwrite(filename.str().c_str(), image1_copy);
#endif
		}
	}

	return 0;
}

void formula1(double& dpq, double Lp, double Lq, double ap, double aq, double bp, double bq)
{
	dpq = 0;
	dpq += pow((Lp - Lq), 2);
	dpq += pow((ap - aq), 2);
	dpq += pow((bp - bq), 2);
	dpq = sqrt(dpq);
}