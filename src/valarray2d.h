#pragma once

#include "stl.h"

template <typename T> class valarray2d {

private:
	std::valarray < std::valarray <T> > _values;

public:

	valarray2d() :
		 _values{}
	{
		initialize(1, 1);
	}
	valarray2d(unsigned int size) :
		_values{}
	{
		initialize(size, size);
	}
	valarray2d(unsigned int initial_rows, unsigned int initial_cols) :
		_values{}
	{
		initialize(initial_rows, initial_cols);
	}
	valarray2d(unsigned int initial_rows, unsigned int initial_cols, T initial_values) :
		_values{}
	{
		initialize(initial_rows, initial_cols);
		flood_values(initial_values);
	}

	~valarray2d() {}

	const unsigned int get_rows()  { return this->_values.size(); }
	const unsigned int get_cols()  { return this->_values[0].size(); }
	const T get_value(unsigned int row, int column) { return this->_values[row][column]; }

	void set_value(int row, int column, T value) { this->_values[row][column] = value; }

	void initialize(unsigned int rows, unsigned int cols);
	void flood_zeros();
	void flood_values(T value);
	void make_identity();

};

template <typename T> void valarray2d <T> ::initialize(unsigned int rows, unsigned int cols)
{
	_values.resize(rows);
	for (unsigned int i{ 0 }; i < cols; i++){ _values[i].resize(cols, static_cast<T>(0.0)) ; }
}

template <typename T> void valarray2d <T> ::flood_zeros()
{
	for (unsigned int i{ 0 }; i < this->get_cols(); i++){ _values[i].resize(this->get_cols(), static_cast<T>(0.0));	}
}

template <typename T> void valarray2d <T> ::flood_values(T value)
{
	for (unsigned int i{ 0 }; i < this->get_cols(); i++){ _values[i].resize(this->get_cols(), static_cast<T>(value)); }
}

template <typename T> void valarray2d <T> ::make_identity()
{
	if (this->get_rows() == this->get_cols())
	{
		for (unsigned int i{ 0 }; i < this->get_rows(); i++){ _values[i][i] = static_cast<T>(1.0); }
	}
}

// TODO: explicit instantiations, class is meant to be used only with these types
/*template class valarray2d<unsigned int>;
template class valarray2d<int>;
template class valarray2d<float>;
template class valarray2d<double>;*/