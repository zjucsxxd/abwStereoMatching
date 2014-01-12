#pragma once

#include "stl.h"

template <class T> class matrix {

private:

	int _rows;
	int _cols;
	std::valarray < std::valarray < T > > _values;

public:

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
	matrix(int initial_rows, int initial_columns, T initial_values) :
		_rows{ initial_rows }, _cols{ initial_columns }, _values{}
	{
		flood_values(initial_values);
	}

	~matrix() {}

	int get_rows()  { return this->_rows; }
	int get_cols()  { return this->_cols; }
	int get_value(int i, int j) { return this->_values[i][j]; }

	void set_value(int i, int j, T value) { this->_values[i][j] = value; }

	void flood_zeros();
	void flood_values(T value);
	void make_identity();

};

template <class T> void matrix <T> ::flood_zeros()
{
	_values.resize(_rows);
	for (int i = 0; i < _cols; i++)
	{
		_values[i].resize(_cols, 0.0);
	}
}

template <class T> void matrix <T> ::flood_values(T value)
{
	_values.resize(_rows);
	for (int i = 0; i < _cols; i++)
	{
		_values[i].resize(_cols, value);
	}
}

template <class T> void matrix <T> ::make_identity()
{
	if (_rows == _cols)
	{
		for (int i = 0; i < _rows; i++)
		{
			_values[i][i] = 1.0;
		}
	}
}