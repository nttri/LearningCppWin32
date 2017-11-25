#pragma once

#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

//-----------------------SHAPE----------------------
class CShape
{
protected:
	Pen* pen;
	Brush *brush;
	Point start;
	Point end;
	int penSize;
	Color outlineColor;
	Color fillColor;
public:
	void Set(Point _bd, Point _kt,int pz,Color oc,Color fc);
	virtual void Draw(Graphics *) = 0;
	CShape();
	~CShape();
};

//-----------------------LINE----------------------
class CLine :public CShape
{
public:
	void Draw(Graphics *);
	CLine();
	~CLine();
};


//--------------------RECTANGLE--------------------
class CRectangle :public CShape
{
public:
	void Draw(Graphics *);
	CRectangle();
	~CRectangle();
};


//---------------------ELLIPSE---------------------
class CEllipse :public CShape
{
public:
	void Draw(Graphics *);
	CEllipse();
	~CEllipse();
};

