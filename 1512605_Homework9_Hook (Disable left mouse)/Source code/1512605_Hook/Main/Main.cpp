// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"

#define MAX_LOADSTRING 100
#define DISABLE 1
#define ENABLE 0

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HINSTANCE g_hDLL;
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAIN));

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      200, 120, 500, 350, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HWND g_hHelp, g_hStatus, btn;
bool checkStatus = ENABLE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			// Lấy font hệ thống
			LOGFONT lf;
			GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			HFONT hFont = CreateFont(20, lf.lfWidth, lf.lfEscapement, lf.lfOrientation,
				lf.lfWeight, lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
				lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
				lf.lfPitchAndFamily, lf.lfFaceName);
			g_hHelp = CreateWindowW(L"STATIC", L"\tPress 'Ctrl' + 'E' \n to Enable/Disable the Left mouse ", WS_CHILD | WS_VISIBLE,
				120, 80, 350, 60, hWnd, NULL, hInst, NULL);

			g_hStatus = CreateWindowW(L"STATIC", L"Status: ENABLE", WS_CHILD | WS_VISIBLE, 180, 180, 250, 25, hWnd, NULL, hInst, NULL);

			btn = CreateWindowW(L"BUTTON", L"Test", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)IDC_BTN, hInst, NULL);

			SendMessage(g_hHelp, WM_SETFONT, WPARAM(hFont), TRUE);
			SendMessage(g_hStatus, WM_SETFONT, WPARAM(hFont), TRUE);
		}
		break;
	case WM_KEYDOWN:
	{
		// kiểm tra user nhấn Ctrl + E
		if (wParam == 0x45)	// 0x45 là 'E'
		{
			int ctrl = GetKeyState(VK_CONTROL);
			if (ctrl & 0x8000) {
				if (checkStatus == DISABLE)		// nếu đang bật hook mà user nhấn Ctrl+E thì tắt hook						
				{
					if (g_hDLL != NULL)
					{
						typedef void(*proc)();
						proc uninstallHook = (proc)GetProcAddress(g_hDLL, "UninstallMouseHook");
						if (uninstallHook != NULL)
						{
							uninstallHook();
						}
						FreeLibrary(g_hDLL);
					}

					// bật thành công
					SetWindowText(g_hStatus, L"Status: ENABLE");
					checkStatus = ENABLE;
				}
				else						// nếu đang tắt hook mà user nhấn Ctrl+E thì bật hook
				{
					g_hDLL = LoadLibrary(L"MyDll.dll");
					typedef  int(*proc1) (HWND);
					if (g_hDLL != NULL)
					{
						proc1 installHookproc = (proc1)GetProcAddress(g_hDLL, "InitMouseHook");
						if (installHookproc != NULL)
						{
							int rs = installHookproc(hWnd);
							if (rs == 1)
							{
								// tắt thành công
								SetWindowText(g_hStatus, L"Status: DISABLE");
								checkStatus = DISABLE;
							}
							else
							{
								SetWindowText(g_hStatus, L"Status: Install hook failed");
							}
						}
						else
						{
							SetWindowText(g_hStatus, L"Status: Get proc failed");
						}
					}
					else
					{
						SetWindowText(g_hStatus, L"Status: Load DLL failed");
					}
				}
			}
		}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_BTN:
			{
				MessageBox(hWnd,L"Left mouse still working",L"Message",MB_OK);
			}
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
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

