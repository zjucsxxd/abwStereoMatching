#pragma once

#include <valarray>

class matrix{

private :
	int _rows;
	int _cols;
	std::valarray < std::valarray < double > > _values;

public :
	matrix() :
		_rows{ 1 }, _cols{ 1 }, _values{}
	{
		flood_zeros();
	}
	matrix(int size) :
		_rows{ size }, _cols{ size }, _values{}
	{
		flood_zeros();
	}
	matrix(int initial_rows, int initial_columns) :
		_rows{ initial_rows }, _cols{ initial_columns }, _values{}
	{
		flood_zeros();
	}
	matrix(int initial_rows, int initial_columns, double initial_values) :
		_rows{ initial_rows }, _cols{ initial_columns }, _values{}
	{
		flood_values(initial_values);
	}

	~matrix() {}

	int get_rows()  { return this->_rows; }
	int get_cols()  { return this->_cols; }
	int get_value(int i, int j) { return this->_values[i][j]; }

	void set_value(int i, int j, int value) { this->_values[i][j] = value; }

	void flood_zeros();
	void flood_values(double value);
	void make_identity();
};