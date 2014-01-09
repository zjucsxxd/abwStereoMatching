#pragma once

#include "cvLibs.h"
#include "matrixio.h"
#include <valarray>

using namespace cv;
using std::valarray;

class adaptiveBinaryWindow{

public :
	adaptiveBinaryWindow() :
		_posx{ 0 }, _posy{ 0 }, _size{ 32 }, _values{} {}
	adaptiveBinaryWindow(int centerPixelX, int centerPixelY) :
		_posx{ centerPixelX }, _posy{ centerPixelY }, _size{ 32 }, _values{} {}
	adaptiveBinaryWindow(int centerPixelX, int centerPixelY, int windowSize) :
		_posx{ centerPixelX }, _posy{ centerPixelY }, _size{ windowSize }, _values{} {}

	int get_posx()  { return _posx; }
	int get_posy() { return _posy; }
	int get_size() { return _size; }
	int get_values(int i, int j) { return _values[i][j]; }

	void set_value(int i, int j, int value) { _values[i][j] = value; }

	void initialize_values();
	// void determine(Mat input);

private :
	int _posx, _posy;
	int _size;
	valarray < valarray < int > > _values;
};