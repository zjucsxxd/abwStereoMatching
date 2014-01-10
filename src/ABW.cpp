#pragma once

#include "ABW.h"

void ABW::initialize_borders()
{
	int size{ get_size() };
	_x1 = _posx - size / 2;
	_y1 = _posy - size / 2;
	_x2 = _posx + size / 2;
	_y2 = _posy + size / 2;
}