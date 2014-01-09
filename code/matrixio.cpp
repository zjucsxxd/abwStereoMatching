#pragma once

#include "matrixio.h"

double readFromLeftImage(Mat &image1_lab, int channel, int i, int j){
	return image1_lab.at<Vec3b>(i, j).val[channel];
}