#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport) 
#else
#define MATHLIBRARY_API __declspec(dllimport) 
#endif

#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


extern "C" MATHLIBRARY_API void CheckOrientDraw(Point &ps, Point &pe);

extern "C" MATHLIBRARY_API void ChangeToSpecial(Point &ps, Point &pe);