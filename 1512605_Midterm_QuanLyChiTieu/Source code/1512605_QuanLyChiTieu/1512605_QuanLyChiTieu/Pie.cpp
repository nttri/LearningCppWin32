#include "stdafx.h"
#include "Pie.h"

Color g_MyColor[7] = { RED,ORANGE,YELLOW,GREEN,BLUE1,BLUE2,PURPLE };

CPie::CPie()
{
}

CPie::CPie(int typeID, REAL radStart, REAL radSize)
{
	ellipseRect = RectF(100, 40, 200, 200);
	pen = new Pen(BLACK, 0);
	brush = new SolidBrush(g_MyColor[typeID]);
	startAngle = radStart;
	sweepAngle = radSize;
}

void CPie::Draw(Graphics *graphics)
{
	graphics->DrawPie(pen, ellipseRect, startAngle, sweepAngle);
	graphics->FillPie(brush, ellipseRect, startAngle, sweepAngle);
}


CPie::~CPie()
{
}
