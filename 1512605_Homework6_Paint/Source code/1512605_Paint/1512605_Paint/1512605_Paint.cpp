// 1512605_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512605_Paint.h"
#include "Line.h"
#include "Rectangle.h"
#include "Ellipse.h"
#pragma warning(disable:4996);

using namespace std;

#define MAX_LOADSTRING 100

//__________________ my Variables ___________________
bool isDrawing = FALSE;				// kiểm tra có đang giữ chuột không
HWND gHPaint;						// thiết bị để vẽ lên
POINT pCurrent, pEnd;				// tọa độ 2 điểm cần thiết để dựng hình
HPEN gHpen;							// bút vẽ
WCHAR *openFileDirec = NULL;		// đường dẫn file cần mở
int isOpenFile = 0;					// kiểm tra mở file
int sizeOfPen = 1;					// mặc định nét vẽ ban đầu là 1 pixel
int iChoose = LINE;					// mặc định ban đầu là vẽ LINE
COLORREF iColor = RGB(0, 0, 0);		// mặc định màu bút ban đầu là đen
vector<CShape*> gShapes;
vector<CShape*> gPrototypes;
HMENU hMenu;
POINT pRMouse;
RECT rect;


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
	LoadStringW(hInstance, IDC_MY1512605_PAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605_PAINT));

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

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605_PAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1512605_PAINT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnButtonUp);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//___________________________________WM function______________________________________
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	hMenu = GetMenu(hWnd);
	// set mặc định ban đầu cho Shape box là Line
	CheckMenuItem(hMenu, ID_SHAPES_LINE, MF_CHECKED);
	// set mặc định ban đầu cho Size box là 1px
	CheckMenuItem(hMenu, ID_SIZE_1PX, MF_CHECKED);

	gPrototypes.push_back(new CLine);
	gPrototypes.push_back(new CRectangle);
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	hMenu = GetMenu(hWnd);

	switch (id)
	{
	case ID_SHAPES_LINE:
	{
		CheckMenuItem(hMenu, ID_SHAPES_LINE, MF_CHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_ELLIPSE, MF_UNCHECKED);
		iChoose = LINE;
		break;
	}
	case ID_SHAPES_RECTANGLE:
	{
		CheckMenuItem(hMenu, ID_SHAPES_LINE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_RECTANGLE, MF_CHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_ELLIPSE, MF_UNCHECKED);
		iChoose = RECTANGLE;
		break;
	}
	case ID_SHAPES_ELLIPSE:
	{
		CheckMenuItem(hMenu, ID_SHAPES_LINE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SHAPES_ELLIPSE, MF_CHECKED);
		iChoose = ELLIPSE;
		break;
	}
	case ID_SIZE_1PX:
	{
		CheckMenuItem(hMenu, ID_SIZE_1PX, MF_CHECKED);
		CheckMenuItem(hMenu, ID_SIZE_5PX, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SIZE_10PX, MF_UNCHECKED);
		sizeOfPen = 1;
		break;
	}
	case ID_SIZE_5PX:
	{
		CheckMenuItem(hMenu, ID_SIZE_1PX, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SIZE_5PX, MF_CHECKED);
		CheckMenuItem(hMenu, ID_SIZE_10PX, MF_UNCHECKED);
		sizeOfPen = 5;
		break;
	}
	case ID_SIZE_10PX:
	{
		CheckMenuItem(hMenu, ID_SIZE_1PX, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SIZE_5PX, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_SIZE_10PX, MF_CHECKED);
		sizeOfPen = 10;
		break;
	}
	case ID_COLORS_CHOOSE:
	{
		iColor = ShowColorDialog(hWnd);
		break;
	}
	case ID_FILE_NEW:
	{
		if (gShapes.size() > 0)
		{
			int mb = MessageBox(hWnd, L"Tạo mới sẽ mất các hình đang có. Bạn chắc chứ?", L"WARNING", MB_OKCANCEL);
			if (mb == 2)			// user không đồng ý xóa
				break;				// để nguyên các hình
		}
		gShapes.clear();
		isOpenFile = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case ID_FILE_OPEN:
	{
		if (gShapes.size() > 0)
		{
			int mb = MessageBox(hWnd, L"Mở file khác sẽ mất các hình đang có. Bạn chắc chứ?", L"WARNING", MB_OKCANCEL);
			if (mb == 2)			// user không đồng ý xóa
				break;				// để nguyên các hình
		}
		gShapes.clear();
		openFileDirec = OpenFileDialog(hWnd);
		int len = wcslen(openFileDirec);
		if (len < 1) break; // tên file lỗi
		isOpenFile = 1; // đánh dấu đang mở file
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case ID_FILE_SAVE:
	{
		WCHAR *saveFileDirec = NULL;
		saveFileDirec = SaveFileDialog(hWnd);
		int len = wcslen(saveFileDirec);

		// Nếu user nhấn cancel/exit sẽ break ra (len=0)
		if (len == 0) break;

		// kiểm tra tên file hợp lệ (đuôi .txt)
		if (len>0)
			if (saveFileDirec[len - 1] != 'p' || saveFileDirec[len - 2] != 'm' || saveFileDirec[len - 3] != 'b' || saveFileDirec[len - 4] != '.')
			{
				MessageBox(hWnd, L"Save thất bại do file không đúng định dạng (.png)", L"ERROR", 0);
				break;
			}

		SaveBMPFile(hWnd, GetDC(hWnd), saveFileDirec);
		break;
	}
	case ID_OPTIONS_UNDO:
	{
		if (gShapes.size() > 0)
		{
			gShapes.pop_back();
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnButtonDown(HWND hwnd, int dc, int x, int y, LPARAM lParam)
{
	if (!isDrawing)
	{
		isDrawing = TRUE;
		pCurrent.x = x;
		pCurrent.y = y;
	}
}

void OnButtonUp(HWND hWnd, int x, int y, LPARAM lParam)
{
	pEnd.x = x;
	pEnd.y = y;

	if (iChoose == LINE)
	{
		CLine* line = new CLine;
		line->SetData(pCurrent.x, pCurrent.y, pEnd.x, pEnd.y, iColor, sizeOfPen);
		gShapes.push_back(line);
	}
	else if (iChoose == RECTANGLE)
	{
		if (lParam & MK_SHIFT)		//Nếu user vẫn giữ SHIFT khi thả chuột thì vẽ H.vuông
			ChangeToSpecial();
		CRectangle* rect = new CRectangle;
		rect->SetData(pCurrent.x, pCurrent.y, pEnd.x, pEnd.y, iColor, sizeOfPen);
		gShapes.push_back(rect);
	}
	else
	{
		if (lParam & MK_SHIFT)		//Nếu user vẫn giữ SHIFT khi thả chuột thì vẽ H.tròn
			ChangeToSpecial();
		CEllipse* ellipse = new CEllipse;
		ellipse->SetData(pCurrent.x, pCurrent.y, pEnd.x, pEnd.y, iColor, sizeOfPen);
		gShapes.push_back(ellipse);
	}

	isDrawing = FALSE;
	InvalidateRect(hWnd, NULL, FALSE);
}

void OnMouseMove(HWND hWnd, int x, int y, LPARAM lParam)
{
	if (isDrawing)
	{
		pEnd.x = x;
		pEnd.y = y;
		if ((lParam & MK_SHIFT) && iChoose != 1)		//Nếu user giữ SHIFT khi kéo chuột thì vẽ H.vuông (Rectangle) / H.tròn (Ellipse) (preview)
			ChangeToSpecial();
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HBRUSH hbr = GetStockBrush(NULL_BRUSH);
	HPEN hpen = CreatePen(0, sizeOfPen, iColor);

	GetClientRect(hWnd, &rect);
	HDC vHdc = CreateCompatibleDC(hdc);
	HBITMAP vBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(vHdc, vBitmap);
	FillRect(vHdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	SelectObject(vHdc, hbr);
	SelectObject(vHdc, hpen);

	if (isOpenFile == 1)	// nếu cờ mở file đang bật
	{
		LoadAndBlitBitmap(openFileDirec, vHdc);	// load lại hình từ file lên để vẽ tiếp lên hình.
	}

	for (int i = 0; i < gShapes.size(); i++) {
		gShapes[i]->Draw(vHdc);
	}

	if (isDrawing) {
		if (iChoose == LINE)
		{
			MoveToEx(vHdc, pCurrent.x, pCurrent.y, NULL);
			LineTo(vHdc, pEnd.x, pEnd.y);
		}
		else if (iChoose == RECTANGLE)
		{
			Rectangle(vHdc, pCurrent.x, pCurrent.y, pEnd.x, pEnd.y);
		}
		else
		{
			Ellipse(vHdc, pCurrent.x, pCurrent.y, pEnd.x, pEnd.y);
		}
	}

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, vHdc, 0, 0, SRCCOPY);
	DeleteObject(hpen);
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}


//___________________________________my function______________________________________

void ChangeToSpecial()
{
	if (abs(pCurrent.x - pEnd.x) > abs(pCurrent.y - pEnd.y))
	{
		if (pCurrent.x > pEnd.x)
			pEnd.x = pCurrent.x - abs(pCurrent.y - pEnd.y);
		else
			pEnd.x = pCurrent.x + abs(pCurrent.y - pEnd.y);
	}
	else
	{
		if (pCurrent.y > pEnd.y)
			pEnd.y = pCurrent.y - abs(pCurrent.x - pEnd.x);
		else
			pEnd.y = pCurrent.y + abs(pCurrent.x - pEnd.x);
	}
}

COLORREF ShowColorDialog(HWND hwnd)
{
	CHOOSECOLOR mColor;
	static COLORREF crCustClr[16];
	ZeroMemory(&mColor, sizeof(mColor));
	mColor.lStructSize = sizeof(mColor);
	mColor.hwndOwner = hwnd;
	mColor.lpCustColors = (LPDWORD)crCustClr;
	mColor.rgbResult = RGB(0, 0, 0);
	mColor.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&mColor);
	return mColor.rgbResult;
}

WCHAR szFile[200];

WCHAR* OpenFileDialog(HWND hwnd)
{
	OPENFILENAMEW ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Bitmap (*.bmp)\0*.bmp\0";
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
	wcscpy(szFile, L"Image01.bmp");
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Bitmap (*.bmp)\0*.bmp\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	ofn.lpstrDefExt = (LPCWSTR)L".bmp";
	ofn.lpstrTitle = L".bmp";
	ofn.lpTemplateName = L".bmp";
	GetSaveFileName(&ofn);
	return ofn.lpstrFile;
}

bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC)
{
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);
	if (hBitmap == NULL) {
		::MessageBox(NULL, __T("Không load được file"), __T("Error"), MB_OK);
		return false;
	}
	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hWinDC);
	if (hLocalDC == NULL) {
		::MessageBox(NULL, __T("Lỗi khởi tạo"), __T("Error"), MB_OK);
		return false;
	}
	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
		reinterpret_cast<LPVOID>(&qBitmap));
	if (!iReturn) {
		::MessageBox(NULL, __T("Lỗi lấy đối tượng"), __T("Error"), MB_OK);
		return false;
	}
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, __T("Đối tượng bị lỗi"), __T("Error"), MB_OK);
		return false;
	}
	BOOL qRetBlit = ::BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight,
		hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit)
	{
		::MessageBox(NULL, __T("Blit lỗi"), __T("Error"), MB_OK);
		return false;
	}

	// Unitialize and deallocate resources
	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return true;
}

void SaveBMPFile(HWND hwnd, HDC hdc, WCHAR* duongdan)
{
	HDC hdc_tam = CreateCompatibleDC(hdc);


	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	SelectObject(hdc_tam, hbmScreen);
	BitBlt(hdc_tam, 0, 0, rcClient.right, rcClient.bottom, hdc, 0, 0, SRCCOPY);
	BITMAP bmpScreen;

	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char *lpbitmap = (char *)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdc, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	// A file is created, this is where we will save the screen capture.
	HANDLE hFile = CreateFile(duongdan,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM   

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	//Close the handle for the file that was created
	CloseHandle(hFile);

}