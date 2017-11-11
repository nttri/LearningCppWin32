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
#pragma comment(lib, "shlwapi.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

using namespace std;

#define MAX_LOADSTRING 200
#define KB 1
#define MB 2
#define GB 3
#define TB 4
#define RADIX 10

/******************************************************************************************/
//khai báo các biến
// tạo handle chính (hwnd) và các handle thành phần (hTree, hList)
extern HWND hwnd, hTree, hList;
// tạo tĩnh 50 chuỗi lưu tên các ổ đĩa
extern WCHAR Diskk[50][3];
// nDisk lưu số lượng ổ đĩa
extern int nDisk;

/*****************************KHAI BÁO CÁC HÀM*************************************/
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
