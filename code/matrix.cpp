#pragma once

#include "matrix.h"

void matrix::flood_zeros()
{
	_values.resize(_rows);
	for (int i = 0; i < _cols; i++)
	{
		_values[i].resize(_cols, 0.0);
	}
}

void matrix::flood_values(double value)
{
	_values.resize(_rows);
	for (int i = 0; i < _cols; i++)
	{
		_values[i].resize(_cols, value);
	}
}

void matrix::make_identity()
{
	if (_rows == _cols)
	{
		for (int i = 0; i < _rows; i++)
		{
			_values[i][i] = 1.0;
		}
	}
}