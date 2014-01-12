#pragma once

#include "matrixio.h"

void readLabPixel(Mat& image, double& L, double& a, double& b, int row, int col)
{
	L = image.at<Vec3b>(row, col).val[0];
	a = image.at<Vec3b>(row, col).val[1];
	b = image.at<Vec3b>(row, col).val[2];
}