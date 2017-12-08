// 1512605_PaintGDIplus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512605_PaintGDIplus.h"
#include "Shape.h"


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr)) {
		return FALSE;
	}

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512605_PAINTGDIPLUS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605_PAINTGDIPLUS));

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

	CoUninitialize();

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;// CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605_PAINTGDIPLUS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = 0;//MAKEINTRESOURCEW(IDC_MY1512605_PAINTGDIPLUS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

ULONG_PTR	token;
GdiplusStartupInput gdiplusStartupInput;

WCHAR *openFileDirec = NULL;		// đường dẫn file cần mở
int isOpenFile = 0;					// kiểm tra mở file
Point pStart, pEnd;					// 2 điểm lưu tọa độ đầu và cuối
vector<CShape*> gShapes;			// mảng lưu các hình đã vẽ
bool onPressed = FALSE;				// kiểm tra nhấn chuột trái
int abc;
int g_shapeType = LINE;					// Loại hình vẽ, mặc định hình ban đầu là Line
int g_penSize = 1;						// Kích thước bút vẽ, mặc định size ban đầu là 1px
Color g_outlineColor = Color(0,0,0);	// Màu viền ngoài, mặc định ban đầu là màu đen
Color g_fillColor;						// Màu phủ bên trong, mặc định ban đầu là không màu
bool g_isFillOn = FALSE;				// kiểm tra người dùng chọn màu fill chưa, ban đầu là False.
int g_RibbonHeight;						// lưu chiều cao thanh ribbon

HINSTANCE g_hinstLib;
MYPROC g_ProcChangeSpecial, g_ProcCheckOrient;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	COLORREF tempOut,tempFill;
	bool initSuccess;
	int len;
	HBITMAP hbitmap;
	HDC hdc[2];
	WCHAR *saveFileDirec = NULL;
	RECT rect;
	int w, h;
	

    switch (message)
    {
	case WM_CREATE:
		initSuccess = InitializeFramework(hWnd);
		if (!initSuccess)
			return -1;
		g_hinstLib = LoadLibrary(L"MyDll.dll");
		if (g_hinstLib) {
			g_ProcChangeSpecial = (MYPROC)GetProcAddress(g_hinstLib, "ChangeToSpecial");
			g_ProcCheckOrient = (MYPROC)GetProcAddress(g_hinstLib, "CheckOrientDraw");
		}
		GdiplusStartup(&token, &gdiplusStartupInput, NULL);
		g_RibbonHeight = GetRibbonHeight();
		break;
	case WM_LBUTTONDOWN:
		if (!onPressed)
		{
			onPressed = true;
			pStart.X = GET_X_LPARAM(lParam);
			pStart.Y = GET_Y_LPARAM(lParam);
			SetCapture(hWnd);
		}
		break;
	case WM_LBUTTONUP:
		pEnd.X = GET_X_LPARAM(lParam);
		pEnd.Y = GET_Y_LPARAM(lParam);
		if (g_shapeType == LINE)
		{
			CLine* line = new CLine;
			line->Set(pStart, pEnd, g_penSize, g_outlineColor, g_fillColor);
			gShapes.push_back(line);
		}
		else if (g_shapeType == RECTANGLE)
		{
			if (wParam & MK_SHIFT)		//Nếu user vẫn giữ SHIFT khi thả chuột thì vẽ H.vuông
				g_ProcChangeSpecial(pStart, pEnd);
			CRectangle* rect = new CRectangle;
			rect->Set(pStart, pEnd, g_penSize, g_outlineColor, g_fillColor);
			gShapes.push_back(rect);
		}
		else
		{
			if (wParam & MK_SHIFT)		//Nếu user vẫn giữ SHIFT khi thả chuột thì vẽ H.tròn
				g_ProcChangeSpecial(pStart, pEnd);
			CEllipse* ellipse = new CEllipse;
			ellipse->Set(pStart, pEnd, g_penSize, g_outlineColor, g_fillColor);
			gShapes.push_back(ellipse);
		}

		onPressed = FALSE;
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if (onPressed)
		{
			pEnd.X = GET_X_LPARAM(lParam);
			pEnd.Y = GET_Y_LPARAM(lParam);
			if ((wParam & MK_SHIFT) && g_shapeType != LINE)		//Nếu user giữ SHIFT khi kéo chuột thì vẽ H.vuông (Rectangle) / H.tròn (Ellipse) (preview)
				g_ProcChangeSpecial(pStart, pEnd);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BTN_NEW:
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
		case ID_BTN_OPEN:
			if (gShapes.size() > 0)
			{
				int mb = MessageBox(hWnd, L"Mở file khác sẽ mất các hình đang có. Bạn chắc chứ?", L"WARNING", MB_OKCANCEL);
				if (mb == 2)			// user không đồng ý xóa
					break;				// để nguyên các hình
			}
			gShapes.clear();
			openFileDirec = OpenFileDialog(hWnd);
			len = wcslen(openFileDirec);
			if (len < 1)
				break;
			isOpenFile = 1; // mở file
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_BTN_SAVE:
			saveFileDirec = SaveFileDialog(hWnd);
			len = wcslen(saveFileDirec);

			// Nếu user nhấn cancel/exit sẽ break ra (len=0)
			if (len == 0) break;

			// kiểm tra tên file hợp lệ (đuôi .txt)
			if (len>0)
				if (saveFileDirec[len - 1] != 'p' || saveFileDirec[len - 2] != 'm' || saveFileDirec[len - 3] != 'b' || saveFileDirec[len - 4] != '.')
				{
					MessageBox(hWnd, L"Lưu file thất bại do file không đúng định dạng (.bmp)", L"ERROR", 0);
					break;
				}

			SaveBMPFile(hWnd, GetDC(hWnd), saveFileDirec);
			MessageBox(hWnd, L"Lưu file thành công!", L"Message", MB_OK);
			break;
		case ID_BTN_EXIT:
			DestroyFramework();
			PostQuitMessage(0);
			break;
		case ID_BTN_SIZE1:
			g_penSize = 1;
			break;
		case ID_BTN_SIZE2:
			g_penSize = 4;
			break;
		case ID_BTN_SIZE3:
			g_penSize = 10;
			break;
		case ID_BTN_LINE:
			g_shapeType = LINE;
			break;
		case ID_BTN_RECT:
			g_shapeType = RECTANGLE;
			break;
		case ID_BTN_ELLIPSE:
			g_shapeType = ELLIPSE;
			break;
		case ID_BTN_OUTLINE:
			tempOut = ShowColorDialog(hWnd);
			g_outlineColor.SetFromCOLORREF(tempOut);
			break;
		case ID_BTN_FILL:
			tempFill = ShowColorDialog(hWnd);
			if (tempFill != 0)	//người dùng đã chọn màu trong color dialog
				g_isFillOn = TRUE;
			g_fillColor.SetFromCOLORREF(tempFill);
			break;
		case ID_BTN_NOFILL:
			g_isFillOn = FALSE;
			break;
		case ID_BTN_UNDO:
			if (gShapes.size() > 0)
				gShapes.pop_back();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		Point ts, te;	// lưu tạm 2 điểm đầu và cuối
		HDC Hdc = BeginPaint(hWnd, &ps);
		Pen* pen = new Pen(g_outlineColor, g_penSize);
		Brush* brush;
		if (g_isFillOn)
			brush = new SolidBrush(g_fillColor);
		else
			brush = new SolidBrush(NULL);

		// TODO: Add any drawing code that uses hdc here...
		HDC Memhdc = CreateCompatibleDC(Hdc);
		GetClientRect(hWnd, &rect);
		HBITMAP hBitmap = CreateCompatibleBitmap(Hdc, rect.right, rect.bottom);
		SelectObject(Memhdc, hBitmap);
		FillRect(Memhdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		Graphics* graphics = new Graphics(Memhdc);
		graphics->SetSmoothingMode(SmoothingModeAntiAlias);

		if (isOpenFile == 1 && openFileDirec != L"")
		{
			LoadAndBlitBitmap(openFileDirec, Memhdc);
		}

		for (int i = 0; i < gShapes.size(); i++)	// vẽ sau khi thả chuột (từ mảng đã lưu)
		{
			gShapes[i]->Draw(graphics);
		}
		if (onPressed)	// vẽ preview
		{
			ts = pStart;
			te = pEnd;
			w = abs(te.X - ts.X);
			h = abs(te.Y - ts.Y);
			if (g_shapeType == LINE)
			{
				graphics->DrawLine(pen, pStart, pEnd);
			}
			else if (g_shapeType == RECTANGLE)
			{
				g_ProcCheckOrient(ts, te);
				graphics->DrawRectangle(pen, ts.X,ts.Y, w,h);
				if (g_penSize != 1)
					graphics->FillRectangle(brush, ts.X + (g_penSize / 2) - 1, ts.Y + (g_penSize / 2) - 1, w - g_penSize + 2, h - g_penSize + 2);
				else
					graphics->FillRectangle(brush, ts.X + (g_penSize / 2), ts.Y + (g_penSize / 2), w - g_penSize+1, h - g_penSize+1);
			}
			else
			{
				g_ProcCheckOrient(ts, te);
				graphics->DrawEllipse(pen, ts.X, ts.Y, w, h);
				if (g_penSize != 1)
					graphics->FillEllipse(brush, ts.X + (g_penSize / 2)-1, ts.Y + (g_penSize / 2)-1, w - g_penSize+2, h - g_penSize+2);
				else
					graphics->FillEllipse(brush, ts.X + (g_penSize / 2), ts.Y + (g_penSize / 2), w - g_penSize + 1, h - g_penSize + 1);
			}
		}
		//BitBlt(hdc, 0, dairibbon, rect.right, rect.bottom, Memhdc, 0, dairibbon, SRCCOPY);
		BitBlt(Hdc, 0, 0, rect.right, rect.bottom, Memhdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		GdiplusShutdown(token);
		DestroyFramework();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//___________________________________my function______________________________________

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
	ofn.lpstrFile[0] = L'\0';
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

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// lấy thuộc tính màu, kích thước của bitmap
	assert(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

	// chuyển định dạng màu thành bits
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	// khởi tạo các thuộc tính cho bitmap  

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP)
{
	HANDLE hf;
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTotal;
	DWORD cb;
	BYTE *hp;
	DWORD dwTmp;
	PBITMAPINFO pbi;
	HDC hDC;

	hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
	SelectObject(hDC, hBMP);

	pbi = CreateBitmapInfoStruct(hBMP);

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	assert(lpBits);
	assert(GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS));

	// Tạo file bmp
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	assert(hf != INVALID_HANDLE_VALUE);

	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);
	assert(WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL));
	assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)));
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	assert(WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL));

	// đóng file bmp  
	assert(CloseHandle(hf));
	// giải phóng  
	GlobalFree((HGLOBAL)lpBits);
}

bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC)
{
	HBITMAP hBitmap;
	int len = wcslen(szFileName);
	if (len == 0)
		return false;
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
	BitBlt(hdc_tam, 0, GetRibbonHeight(), rcClient.right, rcClient.bottom, hdc, 0, GetRibbonHeight(), SRCCOPY);
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
