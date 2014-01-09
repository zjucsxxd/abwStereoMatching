#pragma once

#include "matrix.h"

class ABW : public matrix{

private:
	int _posx;
	int _posy;

public:
	ABW() :
		matrix{}, _posx{ 0 }, _posy{ 0 } {}
	ABW(int centerPixelX, int centerPixelY) :
		matrix{}, _posx{ centerPixelX }, _posy{ centerPixelY } {}
	ABW(int centerPixelX, int centerPixelY, int windowSize) :
		matrix{ windowSize, windowSize }, _posx{ centerPixelX }, _posy{ centerPixelY } {}

	~ABW() {}

	int get_posx()  { return this->_posx; }
	int get_posy() { return this->_posy; }

	void set_posx(int x) { this->_posx = x; };
	void set_posy(int y) { this->_posy = y; };

	int get_size() { return get_rows(); } /// dirty

	/// void markCenterPixel();

	/*for (int i = 0; i < abw.get_size(); i++)
	{
	for (int j = 0; j < abw.get_size(); j++)
	{
	cout << abw.get_values(i, j);
	}
	cout << endl;
	}*/

	/*void nothing()
	{
	int centerx{ this->get_rows() / 2 };
	int centery{ this->get_cols() / 2 };
	}*/
};