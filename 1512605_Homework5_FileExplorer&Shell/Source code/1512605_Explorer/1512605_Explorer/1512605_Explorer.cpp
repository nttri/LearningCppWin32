// 1512605_Explorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512605_Explorer.h"

#pragma warning (disable:4996)

// Global Variables:
HWND hwnd, hContainer, hTree, hList, hStatusBar;
POINT gStartPos;
// tạo tĩnh 50 chuỗi lưu tên các ổ đĩa
WCHAR Diskk[50][3];
// nDisk lưu số lượng ổ đĩa
int nDisk;
int gTVWidth = 0;
int gSplitterMinPos;	// vị trí trái tối đa có thể kéo thanh splitter
int gSplitterMaxPos;	// vị trí phải tối đa có thể kéo thanh splitter
bool gPressing = false;
bool onLVFileClick = false;
WCHAR statusBarText[200];

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512605_EXPLORER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605_EXPLORER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605_EXPLORER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512605_EXPLORER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   WCHAR buffer[255];
   GetCurrentDirectory(255, buffer);
   WCHAR  config[255];
   wsprintf(config, L"%s\\config.ini", buffer);
   WCHAR tam[10];
   int x, y, w, h;
   GetPrivateProfileString(L"app", L"left", L"100", tam, 10, config);
   x = _wtoi(tam);
   GetPrivateProfileString(L"app", L"top", L"100", tam, 10, config);
   y = _wtoi(tam);
   GetPrivateProfileString(L"app", L"right", L"900", tam, 10, config);
   w = _wtoi(tam);
   GetPrivateProfileString(L"app", L"bottom", L"600", tam, 10, config);
   h = _wtoi(tam);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   x, y, w - x, h - y, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnButtonUp);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//=============================== WM_MSG function ===============================

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	HIMAGELIST hImageList;
	HICON hicon;
	HBITMAP hbitmap;
	InitCommonControls();
	RECT rect;
	GetClientRect(hWnd, &rect);
	gTVWidth = rect.right / 3;

	//hContainer = CreateWindow(WC_STATIC, NULL, WS_VISIBLE | WS_CHILD,
	//	0,0,rect.right,rect.bottom - 30,hWnd,NULL,hInst,NULL);

	hTree = CreateWindow(WC_TREEVIEW, _T("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		0, 0, rect.right / 3, rect.bottom, hWnd, (HMENU)IDC_TREEVIEW, hInst, NULL);

	hList = CreateWindow(WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | WS_BORDER | LVS_REPORT,
		(rect.right - rect.left) / 3 + 5, 0, (rect.right - rect.left) * 2 / 3, rect.bottom, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);

	//thêm các bitmap vào list để tạo icon cho Tree View
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hImageList = ImageList_Create(24, 24, ILC_COLOR16, 0, 0);
	ImageList_Add(hImageList, hbitmap, NULL);

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	ImageList_AddIcon(hImageList, hicon);

	SendMessage(hTree, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	SendMessage(hList, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImageList);

	hStatusBar = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, NULL, hInst, NULL);
	SendMessage(hStatusBar, SB_SETMINHEIGHT, 30, 0);

	int partStatusBar[2];
	partStatusBar[0] = 300;
	partStatusBar[1] = -1;
	SendMessage(hStatusBar, SB_SETPARTS, 2, (LPARAM)&partStatusBar);

	Init();
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	gSplitterMaxPos = rect.right - rect.right * 0.1;
	gSplitterMinPos = rect.right*0.1;
	if (gTVWidth <= gSplitterMinPos)
	{
		gTVWidth = gSplitterMinPos + 1;
	}
	if (gTVWidth >= gSplitterMaxPos)
	{
		gTVWidth = gSplitterMaxPos - 1;
	}
	//MoveWindow(hContainer, 0, 0, rect.right, rect.bottom - 30, true);
	MoveWindow(hTree, 0, 0, gTVWidth, rect.bottom - 30, true);
	MoveWindow(hList, gTVWidth + 5, 0, (rect.right - gTVWidth), rect.bottom - 30, true);
	MoveWindow(hStatusBar, 0, rect.bottom - 30, rect.right, 30, 1);
	SendMessage(hStatusBar, SB_SETMINHEIGHT, 30, 0);
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	WCHAR buffer[255];
	GetCurrentDirectory(255, buffer);
	WCHAR  config[255];
	wsprintf(config, L"%s\\config.ini", buffer);
	RECT rect;
	WCHAR tam[10];
	GetWindowRect(hwnd, &rect);
	_itow(rect.left, tam, 10);
	WritePrivateProfileString(L"app", L"left", tam, config);
	_itow(rect.top, tam, 10);
	WritePrivateProfileString(L"app", L"top", tam, config);
	_itow(rect.right, tam, 10);
	WritePrivateProfileString(L"app", L"right", tam, config);
	_itow(rect.bottom, tam, 10);
	WritePrivateProfileString(L"app", L"bottom", tam, config);
	PostQuitMessage(0);
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	WIN32_FIND_DATA findata;
	WORD a;
	WCHAR* tam = L"";

	switch (pnm->idFrom)
	{
	case IDC_TREEVIEW:
		if (pnm->code == NM_DBLCLK)
		{
			TV_ITEM tvItem;
			WCHAR Text[255];
			HTREEITEM selection = (HTREEITEM)TreeView_GetSelection(hTree);
			if (selection == NULL)
				break;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = Text;
			tvItem.cchTextMax = 255;
			tvItem.hItem = selection;

			SendMessage(hTree, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvItem); // get the data!!	
			ListView_DeleteAllItems(hList);
			if (TreeView_GetChild(hTree, selection) == false)
				BrowseFolder((LPWSTR)tvItem.lParam, selection);
			if (wcscmp((LPWSTR)tvItem.lParam, L"This PC") == 0)
				InitListView();
			else
				AddList((LPWSTR)tvItem.lParam);
		}
		break;
	case IDC_LISTVIEW:
		if (pnm->code == NM_DBLCLK)
		{
			int itemVal = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			LVITEM SelectedItem;
			SelectedItem.iItem = itemVal;
			ListView_GetItem(hList, (LVITEM*)&SelectedItem);

			// kiểm tra người dùng có click vào 1 item hay không
			if (SelectedItem.lParam > 0)
				LoadCurSel((LPWSTR)SelectedItem.lParam);
		}
		else if (pnm->code == NM_CLICK)
		{
			int itemVal = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			LVITEM SelectedItem;
			SelectedItem.iItem = itemVal;
			ListView_GetItem(hList, (LVITEM*)&SelectedItem);

			tam = L"";
			tam = (WCHAR*)SelectedItem.lParam;

			// kiểm tra người dùng có click vào 1 item hay không
			if (SelectedItem.lParam > 0)
			{
				if (wcslen(tam) != 2)
				{
					FindFirstFile(tam, &findata);
					SendMessage(hStatusBar, SB_SETICON, 0, (LPARAM)ExtractAssociatedIconW(hInst, tam, &a));
					SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)findata.cFileName);
					WCHAR *tam1 = Convert(findata.nFileSizeLow);
					if (wcslen(tam1) != 0)
					{
						wsprintf(tam, L"Size of file %s", tam1);
						SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)tam);
					}
					else
						SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)L"");
				}
			}
		}
		break;
	}
	return 0;
}

void OnMouseMove(HWND hwnd, int x, int y, LPARAM lParam)
{
	if (x >= gTVWidth && x <= gTVWidth + 5)
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	if (gPressing)
	{
		gTVWidth = gTVWidth + x - gStartPos.x;
		gStartPos.x = x;
		gStartPos.y = y;
		if (gTVWidth <= gSplitterMinPos)
		{
			gTVWidth = gSplitterMinPos + 1;
			return;
		}
		if (gTVWidth >= gSplitterMaxPos)
		{
			gTVWidth = gSplitterMaxPos - 1;
			return;
		}
		SendMessage(hwnd, WM_SIZE, 0, 0);
	}
}

void OnButtonDown(HWND hwnd, int dc, int x, int y, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	gSplitterMaxPos = rect.right - rect.right * 0.1;
	gSplitterMinPos = rect.right*0.1;

	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	if (x >= gTVWidth && x <= gTVWidth + 5)
		gPressing = true;
	gStartPos.x = x;
	gStartPos.y = y;
	SetCapture(hwnd);
}

void OnButtonUp(HWND hwnd, int x, int y, LPARAM lParam)
{
	gPressing = false;
	ReleaseCapture();
}

//=============================== Support function ===============================


void Init()
{
	GetDisk();
	HTREEITEM thisPC = InitThisPC();
	InitTreeView(thisPC);
	InitListViewInterface();
}


void GetDisk()
{
	WCHAR allDiskString[200];
	WCHAR *perDiskString = new WCHAR[3];
	GetLogicalDriveStrings(200, allDiskString);
	int pos = 0;

	// nếu không có ổ đĩa sẽ báo lỗi
	if (wcslen(allDiskString) == 0)
	{
		MessageBox(hwnd, L"Không tìm được ổ đĩa nào. Kiểm tra lại!", L"LỖI", MB_ICONERROR | MB_OK);
		DestroyWindow(hwnd);
		return;
	}

	do
	{
		wcsncpy(perDiskString, allDiskString + pos, 2);
		perDiskString[2] = 0;
		wcscpy(Diskk[nDisk++], perDiskString);
		// nhảy qua 4 ký tự (vd: 'C',':','\\','\0')
		pos += 4;
	} while (allDiskString[pos] != 0);
}


HTREEITEM InitThisPC()
{
	TVINSERTSTRUCT thisPCStruct;
	thisPCStruct.hParent = TVI_ROOT;
	thisPCStruct.hInsertAfter = TVI_FIRST;
	thisPCStruct.item.iImage = 29;
	thisPCStruct.item.iSelectedImage = 29;
	thisPCStruct.item.cchTextMax = 255;
	thisPCStruct.item.lParam = (LPARAM)L"This PC";
	thisPCStruct.item.pszText = L"This PC";
	thisPCStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&thisPCStruct);
}


HTREEITEM AddItemToTreeView(WCHAR *path, HTREEITEM item, WCHAR * folderName)
{
	TVINSERTSTRUCT nodeStruct;
	nodeStruct.hParent = item;
	nodeStruct.hInsertAfter = item;
	// kiểm tra phân loại để chọn icon
	// ổ đĩa thì path chỉ có 2 kí tự (vd: "C:")
	if (path[2] == '\0')
	{
		nodeStruct.item.iImage = 13;
		nodeStruct.item.iSelectedImage = 13;
	}
	// còn lại
	else
	{
		nodeStruct.item.iImage = 31;
		nodeStruct.item.iSelectedImage = 31;
	}
	nodeStruct.item.cchTextMax = 255;
	nodeStruct.item.lParam = (LPARAM)path;
	nodeStruct.item.pszText = folderName;
	nodeStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&nodeStruct);
}


void InitTreeView(HTREEITEM thisPC)
{
	for (int i = 0; i < nDisk; i++)
		AddItemToTreeView(Diskk[i], thisPC, Diskk[i]);
}


void BrowseFolder(WCHAR* parent, HTREEITEM a)
{
	WIN32_FIND_DATA data;
	WCHAR folderPath[MAX_PATH];
	wsprintf(folderPath, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folderPath, &data);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(data.cFileName, L"..") && wcscmp(data.cFileName, L"."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(data.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, data.cFileName);
					AddItemToTreeView(child, a, data.cFileName);
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}


void InitListViewInterface()
{
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;

	lvColumn.cx = 180;
	lvColumn.pszText = _T("Name");
	ListView_InsertColumn(hList, 0, &lvColumn);

	lvColumn.cx = 120;
	lvColumn.pszText = _T("Type");
	ListView_InsertColumn(hList, 1, &lvColumn);

	lvColumn.cx = 80;
	lvColumn.pszText = _T("Size");
	ListView_InsertColumn(hList, 2, &lvColumn);

	lvColumn.cx = 130;
	lvColumn.pszText = _T("Date modified");
	ListView_InsertColumn(hList, 3, &lvColumn);
}


void InitListView()
{
	for (int i = 0; i < nDisk; i++)
		AddDiskToListView(Diskk[i], i);
}


void AddDiskToListView(LPWSTR _disk, int iRows)
{
	long long nSize = 0;
	SHGetDiskFreeSpaceEx(_disk, NULL, (PULARGE_INTEGER)&nSize, NULL);
	LVITEM diskInLV;
	diskInLV.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	diskInLV.iSubItem = 0;
	diskInLV.cchTextMax = 2;
	diskInLV.pszText = _disk;
	diskInLV.iImage = 13;
	diskInLV.iItem = iRows;
	diskInLV.lParam = (LPARAM)_disk;
	ListView_InsertItem(hList, &diskInLV);
	ListView_SetItemText(hList, iRows, 3, L"");
	ListView_SetItemText(hList, iRows, 1, L"Disk");
	ListView_SetItemText(hList, iRows, 2, Convert(nSize));
}


LPWSTR Convert(__int64 nSize)
{
	int nType = 0;
	while (nSize >= 1048576)
	{
		nSize /= 1024;
		++nType;
	}
	__int64 nRight;
	if (nSize >= 1024)
	{
		nRight = nSize % 1024;
		while (nRight > 99)
			nRight /= 10;
		nSize /= 1024;
		++nType;
	}
	else
		nRight = 0;
	TCHAR *buffer = new TCHAR[11];
	_itow_s(nSize, buffer, 11, RADIX);
	if (nRight != 0 && nType > KB)
	{
		wcscat(buffer, L".");
		TCHAR *right = new TCHAR[3];
		_itow_s(nRight, right, 3, RADIX);
		wcscat(buffer, right);
	}
	if (buffer[0] == L'0')
		return L"";
	switch (nType)
	{
	case 0://Bytes
		wcscat(buffer, L" bytes");
		break;
	case KB:
		wcscat(buffer, L" KB");
		break;
	case MB:
		wcscat(buffer, L" MB");
		break;
	case GB:
		wcscat(buffer, L" GB");
		break;
	case TB:
		wcscat(buffer, L" TB");
		break;
	}
	return buffer;
}

#define ENTIRE_STRING NULL

LPWSTR _GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len)
		return _T("File Folder");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}
	RegCloseKey(hKey);

	return pszPath;
}

time_t ConvertTime(const FILETIME& ft)
{
	ULARGE_INTEGER uli;
	uli.LowPart = ft.dwLowDateTime;
	uli.HighPart = ft.dwHighDateTime;
	return uli.QuadPart / 10000000ULL - 11644473600ULL;
}

void AddItemToListView(WCHAR* link, WIN32_FIND_DATA _data, int iRows)
{
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = _data.cFileName;
	if (_data.nFileSizeLow != 0)
		lvItem.iImage = 11;
	else
		lvItem.iImage = 31;
	lvItem.lParam = (LPARAM)link;
	lvItem.cchTextMax = 100;
	lvItem.iItem = iRows;
	ListView_InsertItem(hList, &lvItem);
	time_t Time = ConvertTime(_data.ftLastWriteTime);
	tm *ltm = localtime(&Time);
	WCHAR *wstr = new WCHAR[20];
	if (ltm->tm_hour == 12)
		wsprintf(wstr, L"%d/%d/%d %d:%d PM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min);
	else if (ltm->tm_hour < 12)
		wsprintf(wstr, L"%d/%d/%d %d:%d AM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min);
	else
		wsprintf(wstr, L"%d/%d/%d %d:%d PM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour - 12, ltm->tm_min);
	ListView_SetItemText(hList, iRows, 3, wstr);
	WCHAR* typeStr = _GetType(_data);
	if (typeStr == L"File Folder" && _data.nFileSizeLow != 0)
	{
		ListView_SetItemText(hList, iRows, 1, L"File");
		ListView_SetItemText(hList, iRows, 2, Convert(_data.nFileSizeLow));
	}
	else
	{
		ListView_SetItemText(hList, iRows, 1, _GetType(_data));
		ListView_SetItemText(hList, iRows, 2, Convert(_data.nFileSizeLow));
	}
}

void LoadCurSel(WCHAR *path)
{
	WIN32_FIND_DATA fd;

	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(hList);
		AddList(path);
	}
	else //Nếu là tập tin thì chạy nó
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}

void AddList(LPWSTR parent)
{
	WIN32_FIND_DATA fd;
	WCHAR folder[MAX_PATH];
	wsprintf(folder, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folder, &fd);
	int dem = 0;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (!(fd.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN))
			{
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L".."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(fd.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, fd.cFileName);
					AddItemToListView(child, fd, dem);
					dem++;
				}
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}