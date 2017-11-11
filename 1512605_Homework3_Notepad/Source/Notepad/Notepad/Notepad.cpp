// Notepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Notepad.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100
using namespace std;

// Hàm hỗ trợ
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void ChangeWindowSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
WCHAR* OpenFileDialog(HWND hwnd);
WCHAR* SaveFileDialog(HWND hwnd);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND text;										// main hwnd
bool first = true;								// flag to check the first time of calling ChangeWindowSize()
WCHAR szFile[200];

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
    LoadStringW(hInstance, IDC_NOTEPAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPAD));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOTEPAD);
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
	   250, 100, 800, 500, nullptr, nullptr, hInstance, nullptr);

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
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(22, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	RECT rect;
	GetClientRect(hWnd, &rect);
	text = CreateWindowW(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL,
		0, 0, rect.right, rect.bottom, hWnd, NULL, hInst, NULL);
	SendMessage(text, WM_SETFONT, WPARAM(hFont), TRUE);
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR *fileDirec = NULL, *buffer = NULL;
	int len = 0;
	const char* c;
	_bstr_t b;
	wifstream fin;
	wofstream fout;
	wstring wstr, wline;
	const wchar_t* content = NULL;

	switch (id)
	{
	case ID_FILE_OPENFILE:
		// Mở open file dialog
		fileDirec = OpenFileDialog(hWnd);
		len = wcslen(fileDirec);

		// Nếu user nhấn cancel sẽ break ra (len=0)
		if (len == 0) break;

		fin.open(fileDirec);
		fin.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

		while (!fin.eof())
		{
			getline(fin, wline);
			if (!fin.eof())
				wstr += wline + L"\r\n";
			else
				wstr += wline;
		}
		//wstr = wstring(str.begin(),str.end());
		content = wstr.c_str();
		SetWindowText(text, content);
		fin.close();
		break;

	case ID_FILE_SAVEFILE:
		len = GetWindowTextLength(text);
		buffer = new WCHAR[len + 1];
		GetWindowText(text, buffer, len + 1);

		fileDirec = SaveFileDialog(hWnd);
		len = wcslen(fileDirec);

		// kiểm tra tên file hợp lệ (đuôi .txt)
		if (len>0)
			if (fileDirec[len - 1] != 't' || fileDirec[len - 2] != 'x' || fileDirec[len - 3] != 't' || fileDirec[len - 4] != '.')
			{
				MessageBox(hWnd, L"Save thất bại do File không đúng định dạng (.txt)", L"ERROR", 0);
				break;
			}

		b = buffer;
		c = b;

		fout.open(fileDirec);
		fout.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));
		fout << buffer;
		fout.close();
		break;

	case ID_EDIT_COPY:
		SendMessage(text, WM_COPY, 0, 0);
		break;

	case ID_EDIT_CUT:
		SendMessage(text, WM_CUT, 0, 0);
		break;

	case ID_EDIT_PASTE:
		SendMessage(text, WM_PASTE, 0, 0);
		break;

	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}

	if (!fileDirec)
		delete[] fileDirec;
}

void ChangeWindowSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!first)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MoveWindow(text, 0, 0, rect.right, rect.bottom, false);
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

WCHAR* OpenFileDialog(HWND hwnd)
{
	OPENFILENAMEW ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Documents (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	return ofn.lpstrFile;
}

WCHAR* SaveFileDialog(HWND hwnd)
{
	OPENFILENAMEW ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Documents (*.txt)\0*.txt\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	ofn.lpstrDefExt = (LPCWSTR)L".txt";
	ofn.lpstrTitle = L".txt";
	ofn.lpTemplateName = L".txt";
	GetSaveFileName(&ofn);
	return ofn.lpstrFile;
}