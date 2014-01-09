#pragma once

#include "adaptiveBinaryWindow.h"

void adaptiveBinaryWindow::initialize_values(){
	_values.resize(_size);
	for (int i = 0; i < _size; i++)
	{
		_values[i].resize(_size, 0);
	}
}

// void adaptiveBinaryWindow::determine(cv::Mat input){}