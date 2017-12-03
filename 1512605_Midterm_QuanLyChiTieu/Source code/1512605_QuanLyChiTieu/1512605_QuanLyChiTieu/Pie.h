#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#define BLACK Color(0,0,0)
#define RED Color(255,0,0)
#define ORANGE Color(255,127,0)
#define YELLOW Color(255,255,0)
#define GREEN Color(0,255,0)
#define BLUE1 Color(0,153,255)
#define BLUE2 Color(0,0,255)
#define PURPLE Color(171, 15, 219)

class CPie
{
private:
	Pen *pen;
	SolidBrush *brush;
	Color outlineColor;
	Color fillColor;
	RectF ellipseRect;
	REAL startAngle;
	REAL sweepAngle;
public:
	CPie();
	CPie(int, REAL, REAL);
	void Draw(Graphics*);
	~CPie();
};

