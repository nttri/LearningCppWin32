#include "stdafx.h"
#include "MatLib.h"
#include <iostream>


void CheckOrientDraw(Point &ps, Point &pe)
{
	if (pe.X < ps.X && pe.Y < ps.Y)
		std::swap(ps, pe);
	else if (ps.X < pe.X && pe.Y < ps.Y)
		std::swap(ps.Y, pe.Y);
	else if (ps.X > pe.X && ps.Y < pe.Y)
		std::swap(ps.X, pe.X);
	else;
}

void ChangeToSpecial(Point &ps, Point &pe)
{
	if (abs(ps.X - pe.X) > abs(ps.Y - pe.Y))
	{
		if (ps.X > pe.X)
			pe.X = ps.X - abs(ps.Y - pe.Y);
		else
			pe.X = ps.X + abs(ps.Y - pe.Y);
	}
	else
	{
		if (ps.Y > pe.Y)
			pe.Y = ps.Y - abs(ps.X - pe.X);
		else
			pe.Y = ps.Y + abs(ps.X - pe.X);
	}
}