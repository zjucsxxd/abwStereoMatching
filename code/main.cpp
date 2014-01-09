#include "cvLibs.h"
#include "adaptiveBinaryWindow.h"
#include <iostream>

using namespace cv;
using std::cout;
using std::endl;

int main (int argc, char** argv)
{

	/// input
	Mat image1 = imread(argv[1]);
	Mat image2 = imread(argv[2]);
	
	imwrite("image1.png", image1);
	
	/// CIELab conversion
	Mat image1_lab;
	Mat image2_lab;
	cvtColor(image1, image1_lab, CV_RGB2Lab);
	cvtColor(image2, image2_lab, CV_RGB2Lab);

	// ABW construction
	adaptiveBinaryWindow abw{512, 512, 256};
	abw.initialize_values();

	/*for (int i = 0; i < abw.get_size(); i++)
	{
		for (int j = 0; j < abw.get_size(); j++)
		{
			cout << abw.get_values(i, j);
		}
		cout << endl;
	}*/

	// ABW shape determination

	int size{ abw.get_size() };

	double Lp = readFromLeftImage(image1_lab, 0, abw.get_posx(), abw.get_posy());
	double ap = readFromLeftImage(image1_lab, 1, abw.get_posx(), abw.get_posy());
	double bp = readFromLeftImage(image1_lab, 2, abw.get_posx(), abw.get_posy());

	int topEdge{ abw.get_posy() + size / 2 };
	int leftEdge{ abw.get_posx() - size / 2 };
	double threshold{ 1 };

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			double Lq = readFromLeftImage(image1_lab, 0, leftEdge + i, topEdge - j);
			double aq = readFromLeftImage(image1_lab, 1, leftEdge + i, topEdge - j);
			double bq = readFromLeftImage(image1_lab, 2, leftEdge + i, topEdge - j);

			double dpq{0};
			dpq += pow((Lp - Lq), 2);
			dpq += pow((ap - aq), 2);
			dpq += pow((bp - bq), 2);
			dpq = sqrt(dpq);

			if (dpq < threshold){ abw.set_value(i, j, 255); }
		}
	}

	// ABW overlay creation

	int topEdge1{ abw.get_posy() + size / 2 };
	int leftEdge1{ abw.get_posx() - size / 2 };

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			image1.at<Vec3b>(leftEdge1 + i, topEdge1 - j).val[0] = abw.get_values(i, j);
			image1.at<Vec3b>(leftEdge1 + i, topEdge1 - j).val[1] = abw.get_values(i, j);
			image1.at<Vec3b>(leftEdge1 + i, topEdge1 - j).val[2] = abw.get_values(i, j);
		}
	}

	imwrite("image1_abw.png", image1);

	return 0;
}