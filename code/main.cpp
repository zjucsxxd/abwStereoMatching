#include "cvLibs.h"
#include "matrix.h"
#include "ABW.h"
#include "matrixio.h"
#include <iostream>

//#define DRAW_OVERLAY

using std::cout;
using std::endl;
using std::stringstream;

void formula1(double& dpq, double Lp, double Lq, double ap, double aq, double bp, double bq); /// dirty

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
	int const bucketSize{ 16 };

	/// TODO: for each pixel of image1
	for (int i{ bucketSize / 2 }; i < image1.rows - bucketSize / 2; i+=bucketSize)
	{
		for (int j{ bucketSize / 2 }; j < image1.cols - bucketSize / 2; j += bucketSize)
		{
			/// bucket construction
			ABW testWindow{ i, j, bucketSize };

			int xp{ testWindow.get_posx() };
			int yp{ testWindow.get_posy() };
			int* xp_ptr{ &xp };
			int* yp_ptr{ &yp };

			double Lp{};
			double ap{};
			double bp{};

			/// ABW shape determination
			readLabPixel(image1_lab, Lp, ap, bp, xp, yp);

			/// for each pixel of bucket
			int yq{ yp + bucketSize / 2 };
			int xq{ xp - bucketSize / 2 };
			//int* xq_ptr{ &xq };
			//int* yq_ptr{ &yq };

			double Lq{};
			double aq{};
			double bq{};

			double dpq{};

#ifdef DRAW_OVERLAY
			Mat image1_copy{ image1.clone() };
#endif
			for (int k = 0; k < bucketSize; k++)
			{
				for (int l = 0; l < bucketSize; l++)
				{
					readLabPixel(image1_lab, Lq, aq, bq, xq + k, yq - l);
					formula1(dpq, Lp, Lq, ap, aq, bp, bq);
					if (dpq < threshold){ testWindow.set_value(k, l, 255); };
#ifdef DRAW_OVERLAY
					image1_copy.at<Vec3b>(xq + k, yq - l).val[0] = testWindow.get_value(k, l);
					image1_copy.at<Vec3b>(xq + k, yq - l).val[1] = testWindow.get_value(k, l);
					image1_copy.at<Vec3b>(xq + k, yq - l).val[2] = testWindow.get_value(k, l);
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