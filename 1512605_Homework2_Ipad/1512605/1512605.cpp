// 1512605.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512605.h"
#include <comdef.h>
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);


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
    LoadStringW(hInstance, IDC_MY1512605, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512605);
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
	   350, 200, 500, 250, nullptr, nullptr, hInstance, nullptr);

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

#define CreateTextBox(text, left, top, width, height, ID) 

HWND num1;
HWND num2;
HWND res;

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HWND hwnd;

	hwnd = CreateWindowW(L"STATIC", L"Nhập giá IPAD ở Nhật: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 30, 150, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowW(L"STATIC", L"Nhập giá IPAD ở Singapore: ", WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 80, 150, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	num1 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_RIGHT, 200, 30, 200, 25, hWnd, NULL, hInst, NULL);
	SendMessage(num1, WM_SETFONT, WPARAM(hFont), TRUE);

	num2 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_RIGHT, 200, 80, 200, 25, hWnd, NULL, hInst, NULL);
	SendMessage(num2, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowW(L"STATIC", L"JPY", WS_CHILD | WS_VISIBLE | SS_LEFT, 410, 30, 30, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowW(L"STATIC", L"SGD", WS_CHILD | WS_VISIBLE | SS_LEFT, 410, 80, 30, 25, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	CreateWindowW(L"BUTTON", L"SO SÁNH", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 340, 130, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR* buffer1 = NULL;
	WCHAR* buffer2 = NULL;
	WCHAR* bufferKQ = NULL;

	int size1;
	int size2;
	int sizeKQ;

	long long giaNhat=0;
	long long giaSing=0;
	long long chenhLech = 0;

	bool flag = true;

	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDC_BUTTON1:
		size1 = GetWindowTextLength(num1);
		size2 = GetWindowTextLength(num2);

		buffer1 = new WCHAR[size1 + 1];
		buffer2 = new WCHAR[size2 + 1];

		GetWindowText(num1, buffer1, size1 + 1);
		GetWindowText(num2, buffer2, size2 + 1);

		_bstr_t bf1(buffer1);
		_bstr_t bf2(buffer2);
		bufferKQ = new WCHAR[255];

		// kiểm tra đầu vào hợp lệ hay không
		char* check1 = bf1;
		char* check2 = bf2;
		// kiểm tra chuỗi rỗng
		if (size1 == 0 || size2 == 0)
		{
			wsprintf(bufferKQ, L"Chuỗi rỗng. Hãy nhập số tiền.");
			MessageBox(hWnd, bufferKQ, L"CẢNH BÁO", 0);
			break;
		}

		// kiểm tra chuỗi chứa kí tự khác số
		for (int i = 0; i<size1; i++)
			if (check1[i]<'0' || check1[i]>'9')
			{
				wsprintf(bufferKQ, L"Chỉ nhập số. Hãy kiểm tra lại.");
				MessageBox(hWnd, bufferKQ, L"CẢNH BÁO", 0);
				flag = false;
				break;
			}
		if (!flag)
			break;

		for (int i = 0; i<size2; i++)
			if (check1[i]<'0' || check1[i]>'9')
			{
				wsprintf(bufferKQ, L"Chỉ nhập số. Hãy kiểm tra lại.");
				MessageBox(hWnd, bufferKQ, L"CẢNH BÁO", 0);
				flag = false;
				break;
			}
		if (!flag)
			break;

		giaNhat = _wtoi(buffer1);
		giaSing = _wtoi(buffer2);

		giaNhat = giaNhat * 203;	// 1 JPY = 203 đồng VN
		giaSing = giaSing * 16872;	// 1 SGD = 16872 đồng VN

		chenhLech = abs(giaNhat - giaSing);

		if (giaNhat > giaSing)
			wsprintf(bufferKQ, L"Nên mua ở Singapore để tiết kiệm %d vnd", chenhLech);
		else if (giaNhat < giaSing)
			wsprintf(bufferKQ, L"Nên mua ở Nhật để tiết kiệm %d vnd", chenhLech);
		else
			wsprintf(bufferKQ, L"Mua ở đâu cũng đồng giá nhau thôi\n(Nhưng để cho gần thì nên qua Singapore haha)");

		MessageBox(hWnd, bufferKQ, L"LỜI KHUYÊN", 0);
		break;
	}

	if (!buffer1)
		delete[] buffer1;
	if (!buffer2)
		delete[] buffer2;
	if (!bufferKQ)
		delete[] bufferKQ;
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