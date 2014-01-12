#pragma once

#include "valarray2d.h"

template <class T> class ABW : public valarray2d <T> {

private:

	int _posx;
	int _posy;
	int _x1, _y1, _x2, _y2;

public:

	ABW() :
		valarray2d{}, _posx{ 0 }, _posy{ 0 }
	{
		initialize_borders();
	}
	ABW(int centerPixelX, int centerPixelY) :
		valarray2d{}, _posx{ centerPixelX }, _posy{ centerPixelY }
	{
		initialize_borders();
	}
	ABW(int centerPixelX, int centerPixelY, unsigned int windowSize) :
		valarray2d{ windowSize, windowSize }, _posx{ centerPixelX }, _posy{ centerPixelY }
	{
		initialize_borders();
	}

	~ABW() {}

	int get_posx()  { return this->_posx; }
	int get_posy() { return this->_posy; }
	int get_size() { return get_rows(); }
	int get_x1() { return this->_x1; }
	int get_y1() { return this->_y1; }
	int get_x2() { return this->_x2; }
	int get_y2() { return this->_y2; }

	void set_posx(int x) { this->_posx = x; };
	void set_posy(int y) { this->_posy = y; };
	void set_x1(int value) { this->_x1 = value; }
	void set_y1(int value) { this->_x1 = value; }
	void set_x2(int value) { this->_x1 = value; }
	void set_y2(int value) { this->_x1 = value; }

	void initialize_borders();
	void markCenterPixel();
	void print_values();
};

template <class T> void ABW <T> ::initialize_borders()
{
	int size{ get_size() };
	_x1 = _posx - size / 2;
	_y1 = _posy - size / 2;
	_x2 = _posx + size / 2;
	_y2 = _posy + size / 2;
}

template <class T> void ABW <T> ::print_values()
{
	for (int i = 0; i < get_size(); i++)
	{
		for (int j = 0; j < get_size(); j++)
		{
			std::cout << get_value(i, j);
		}
		std::cout << std::endl;
	}
}