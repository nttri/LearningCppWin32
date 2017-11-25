#pragma once

#include "resource.h"
#include <windowsx.h>
#include <windowsX.h>
#include <assert.h>
#include <comdef.h>
#include "RibbonFramework.h"
#include "RibbonIDs.h"
#include "Shape.h"
#include <vector>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")

#define MAX_LOADSTRING 100

using namespace std;

//---------
#define LINE 1
#define RECTANGLE 2
#define ELLIPSE 3

//_________________ global variables____________________

extern int g_shapeType;				// Loại hình vẽ, mặc định hình ban đầu là đường thẳng
extern int g_penSize;				// Kích thước bút vẽ, mặc định size ban đầu là 1px
extern Color g_outlineColor;		// Màu viền ngoài, mặc định ban đầu là màu đen
extern Color g_fillColor;			// Màu phủ bên trong, mặc định ban đầu là không màu
extern bool g_isFillOn;				// kiểm tra người dùng chọn màu fill chưa, ban đầu là False
extern int g_RibbonHeight;			// lưu chiều cao thanh ribbon

//____________________ my Function _____________________
void ChangeToSpecial();
COLORREF ShowColorDialog(HWND hwnd);
WCHAR* OpenFileDialog(HWND hwnd);
WCHAR* SaveFileDialog(HWND hwnd);
PBITMAPINFO CreateBitmapInfoStruct(HBITMAP);
void CheckOrientDraw(Point &ps,Point &pe);
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP);
bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC);
void SaveBMPFile(HWND hwnd, HDC hdc, WCHAR* duongdan);

