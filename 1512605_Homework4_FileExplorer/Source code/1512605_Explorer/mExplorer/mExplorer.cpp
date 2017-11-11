// mExplorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mExplorer.h"


// Global Variables:
HWND hwnd, hTree, hList;
WCHAR Diskk[50][3];
int nDisk;
bool first = true;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void ChangeWindowSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);


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
    LoadStringW(hInstance, IDC_MEXPLORER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEXPLORER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEXPLORER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MEXPLORER);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   120, 60, 800, 600, nullptr, nullptr, hInstance, nullptr);

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
		HANDLE_MSG(hWnd, WM_SIZE, ChangeWindowSize);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	HIMAGELIST hImageList;
	HICON hicon;
	HBITMAP hbitmap;

	InitCommonControls();
	RECT rect;
	GetClientRect(hWnd, &rect);

	hTree = CreateWindowW(WC_TREEVIEW, _T("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		0, 0, rect.right / 3, rect.bottom, hWnd, (HMENU)IDC_TREEVIEW, hInst, NULL);
	hList = CreateWindowW(WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | WS_BORDER | LVS_REPORT,
		(rect.right - rect.left) / 3, 0, (rect.right - rect.left) * 2 / 3, rect.bottom, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);

	//thêm các bitmap vào list để tạo icon cho Tree View
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hImageList = ImageList_Create(24, 24, ILC_COLOR16, 0, 0);
	ImageList_Add(hImageList, hbitmap, NULL);

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	ImageList_AddIcon(hImageList, hicon);

	SendMessage(hTree, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	SendMessage(hList, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImageList);

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

void ChangeWindowSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!first)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MoveWindow(hTree, 0, 0, rect.right / 3, rect.bottom, false);
		MoveWindow(hList, (rect.right - rect.left) / 3, 0, (rect.right - rect.left) * 2 / 3, rect.bottom, false);
	}
	first = false;
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
	PostQuitMessage(0);
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
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
		break;
	}
	return 0;
}