#include "stdafx.h"
#include "Shape.h"
#include "1512605_PaintGDIplus.h"

//-----------------------------SHAPE-----------------------------
CShape::CShape()
{
}

void CShape::Set(Point _bd, Point _kt,int pz,Color oc, Color fc)
{
	pen = new Pen(oc, pz);
	if (g_isFillOn)
		brush = new SolidBrush(fc);
	else
		brush = new SolidBrush(NULL);
	start = _bd;
	end = _kt;
	penSize = pz;
	outlineColor = oc;
	fillColor = fc;
}

CShape::~CShape()
{
}

//-----------------------------LINE-----------------------------
void CLine::Draw(Graphics *graphics)
{
	graphics->DrawLine(pen, start, end);
}

CLine::CLine()
{
}

CLine::~CLine()
{
}

//-----------------------------RECT-----------------------------
void CRectangle::Draw(Graphics *graphics)
{
	//CheckOrientDraw(start, end);==============================================================
	g_ProcCheckOrient(start, end);
	graphics->DrawRectangle(pen, start.X, start.Y, end.X - start.X, end.Y - start.Y);
	if(penSize!=1)
		graphics->FillRectangle(brush, start.X+(penSize/2)-1, start.Y+(penSize/2)-1, end.X - start.X-penSize+2, end.Y - start.Y-penSize+2);
	else
		graphics->FillRectangle(brush, start.X + (penSize / 2), start.Y + (penSize / 2), end.X - start.X - penSize + 1, end.Y - start.Y - penSize + 1);
}

CRectangle::CRectangle()
{
}

CRectangle::~CRectangle()
{
}

//-----------------------------ELLIP-----------------------------
void  CEllipse::Draw(Graphics *graphics)
{
	//CheckOrientDraw(start, end);=============================================================
	g_ProcCheckOrient(start, end);
	graphics->DrawEllipse(pen, start.X, start.Y, end.X - start.X, end.Y - start.Y);
	if (penSize != 1)
		graphics->FillEllipse(brush, start.X + (penSize / 2) - 1, start.Y + (penSize / 2) - 1, end.X - start.X - penSize + 2, end.Y - start.Y - penSize + 2);
	else
		graphics->FillEllipse(brush, start.X + (penSize / 2), start.Y + (penSize / 2), end.X - start.X - penSize + 1, end.Y - start.Y - penSize + 1);

}

CEllipse::CEllipse()
{
}

CEllipse::~CEllipse()
{
}