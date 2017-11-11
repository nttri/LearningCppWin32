﻿#pragma once

#include "resource.h"
#include <iostream>
#include <vector>
#include <windowsx.h>
#include <windowsX.h>
#include <assert.h>
#include <comdef.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

#define LINE 1
#define RECTANGLE 2
#define ELLIPSE 3

//__________________ Variables ___________________
extern bool isDrawing;				// kiểm tra có đang giữ chuột không
extern HWND gHPaint;				// thiết bị để vẽ lên
extern POINT pCurrent, pEnd;		// tọa độ 2 điểm cần thiết để dựng hình
extern HPEN gHpen;					// bút vẽ
extern WCHAR *openFileDirec;		// đường dẫn file cần mở
extern int isOpenFile;				// kiểm tra mở file
extern int sizeOfPen;				// mặc định nét vẽ ban đầu là 1 pixel
extern int iChoose;					// mặc định ban đầu là vẽ LINE
extern COLORREF iColor;				// mặc định màu bút ban đầu là đen

//____________________ my Function _____________________
void ChangeToSpecial();
COLORREF ShowColorDialog(HWND hwnd);
WCHAR* OpenFileDialog(HWND hwnd);
WCHAR* SaveFileDialog(HWND hwnd);
PBITMAPINFO CreateBitmapInfoStruct(HBITMAP);
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP);
bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC);

//____________________WM function______________________
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnButtonDown(HWND hwnd, int dc, int x, int y, LPARAM lParam);
void OnButtonUp(HWND hwnd, int x, int wParam, LPARAM lParam);
void OnMouseMove(HWND hwnd, int x, int y, LPARAM lParam);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);