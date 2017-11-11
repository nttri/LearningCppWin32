#pragma once

#include "stdafx.h"
#include "resource.h"
#include <Shellapi.h>
#include <Windows.h>
#include <windowsX.h>
#include <time.h>
#include <CommCtrl.h>
#include <shlwapi.h>
#include <exception>
#include <Shobjidl.h>
#include <Shlobj.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment (lib,"Shell32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 200
#define KB 1
#define MB 2
#define GB 3
#define TB 4
#define RADIX 10

//----WM_MSG function
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
void OnButtonDown(HWND hwnd, int dc, int x, int y, LPARAM lParam);
void OnButtonUp(HWND hwnd, int x, int y, LPARAM lParam);
void OnMouseMove(HWND hwnd, int x, int y, LPARAM lParam);


//----Support function
void GetDisk();
void Init();

// các hàm phục vụ cho Tree View
void InitTreeView(HTREEITEM thisPC);
HTREEITEM InitThisPC();
HTREEITEM AddItemToTreeView(WCHAR *s, HTREEITEM a, WCHAR * ht);
void BrowseFolder(WCHAR* parent, HTREEITEM a);

// các hàm phục vụ cho List View
void InitListViewInterface();
void InitListView();
void LoadCurSel(WCHAR * path);
LPWSTR Convert(__int64 nSize);
time_t ConvertTime(const FILETIME& ft);
LPWSTR _GetType(const WIN32_FIND_DATA &fd);
void AddDiskToListView(LPWSTR _disk, int iRows);
void AddItemToListView(WCHAR* link, WIN32_FIND_DATA _data, int iRows);
void AddList(LPWSTR parent);