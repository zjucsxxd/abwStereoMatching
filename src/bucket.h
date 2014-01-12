#pragma once

#include "valarray2d.h"

template <typename T> class bucket : public valarray2d <T> {

private:

	unsigned int _posx, _posy;
	int _x1, _y1, _x2, _y2;

public:

	bucket() :
		valarray2d{}, _posx{ 0 }, _posy{ 0 }
	{
		initialize_borders();
	}
	bucket(unsigned int row, unsigned int column) :
		valarray2d{}, _posx{ column }, _posy{ row }
	{
		initialize_borders();
	}
	bucket(unsigned int row, unsigned int column, unsigned int bucketSize) :
		valarray2d{ bucketSize, bucketSize }, _posx{ column }, _posy{ row }
	{
		initialize_borders();
	}

	~bucket() {}

	const unsigned int get_posx()  { return this->_posx; }
	const unsigned int get_posy() { return this->_posy; }
	const unsigned int get_size() { return this->get_rows(); }
	const int get_x1() { return this->_x1; }
	const int get_y1() { return this->_y1; }
	const int get_x2() { return this->_x2; }
	const int get_y2() { return this->_y2; }

	void set_posx(unsigned int column) { this->_posx = column; };
	void set_posy(unsigned int row) { this->_posy = row; };
	void set_x1(int value) { this->_x1 = value; }
	void set_y1(int value) { this->_x1 = value; }
	void set_x2(int value) { this->_x1 = value; }
	void set_y2(int value) { this->_x1 = value; }

	void initialize_borders();
};

template <typename T> void bucket <T> ::initialize_borders()
{
	const unsigned int size{ this->get_size() };
	_x1 = _posx - size / 2;
	_y1 = _posy - size / 2;
	_x2 = _posx + size / 2;
	_y2 = _posy + size / 2;
}