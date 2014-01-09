#pragma once

#include "matrixio.h"

void readLabPixel(Mat& image, double& L, double& a, double& b, int x, int y)
{
	//int rows{ image.rows };
	//int cols{ image.cols };
	//for (int i{ 0 }; i < rows; i++)
	//{
		//for (int j{ 0 }; j < cols; j++)
		//{
			L = image.at<Vec3b>(x, y).val[0];
			a = image.at<Vec3b>(x, y).val[1];
			b = image.at<Vec3b>(x, y).val[2];
		//}
	//}
}