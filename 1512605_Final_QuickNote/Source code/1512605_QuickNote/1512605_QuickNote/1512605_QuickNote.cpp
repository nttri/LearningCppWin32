// 1512605_QuickNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CNote.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND g_HWND1, g_HWND2, g_HWND3;					// các cửa sổ
int g_onWindow = 0;								// lưu cửa sổ hiện tại
HHOOK g_hook;
HWND g_btnNewNote, g_btnStatistics, g_tvTags, g_lvNotes, g_btnBack, g_btnSaveNote, g_btnDeleteNote,
		g_tfTitle, g_tfTags, g_tfContent;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
vector<CTag*> g_tagsList;						// mảng lưu các tag
vector<CNote*> g_notesList;						// mảng lưu các note
int g_noteID = 0;								// tạo ID cho các note
HTREEITEM g_treeViewRoot;						// gốc của treeview
wstring g_TagFile = L"resource\\Tag.dat";		// tên file dữ liệu chứa các tag
wstring g_NoteFile = L"resource\\Note.dat";		// tên file dữ liệu chứa các note
bool g_Added = false;							// đánh dấu note đang nhập chưa được lưu
int g_currentIDNote = 0;						// ID của note được lấy từ listview
bool g_openFromLV = false;						// đánh dấu note được mở từ listview

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc3(HWND, UINT, WPARAM, LPARAM);
vector<CTag*> getTopFiveTag();		// lấy ra tối đa 5 tag nhiều note nhất nếu có

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
    LoadStringW(hInstance, IDC_MY1512605QUICKNOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605QUICKNOTE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		/*if (msg.message == WM_KEYDOWN && msg.wParam == 13 && g_onWindow != 2)
		{
			ShowWindow(g_HWND2, 1);
			g_onWindow = 2;
		}*/
		if (IsDialogMessage(g_HWND2, &msg) == false)
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
    wcex.lpfnWndProc    = WndProc1;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605QUICKNOTE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512605QUICKNOTE);
    wcex.lpszClassName  = L"HWND1";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.lpfnWndProc = WndProc3;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605QUICKNOTE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1512605QUICKNOTE);
	wcex.lpszClassName = L"HWND3";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.lpfnWndProc = WndProc2;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605QUICKNOTE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MY1512605QUICKNOTE);
	wcex.lpszClassName = L"HWND2";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   g_HWND1 = CreateWindowW(L"HWND1", L"QUICK NOTE", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      150, 50, 1200, 750, nullptr, nullptr, hInstance, nullptr);

   g_HWND2 = CreateWindowW(L"HWND2", L"New note", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
	   400, 150, 800, 550, nullptr, nullptr, hInstance, nullptr);

   g_HWND3 = CreateWindowW(L"HWND3", L"Statistic", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
	   500, 100, 600, 650, nullptr, nullptr, hInstance, nullptr);

   if (!g_HWND1)
   {
      return FALSE;
   }

   ShowWindow(g_HWND1, 0);
   ShowWindow(g_HWND2, 0);
   ShowWindow(g_HWND3, 0);
   UpdateWindow(g_HWND1);
   AddNotificationIcon(g_HWND1, hInst);
   return TRUE;
}

LRESULT CALLBACK WndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate1);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand1);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy1);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint1);
		HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem1);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
	case ID_NOTI_AREA:
	{
		switch (lParam)
		{
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		{
			HMENU NotifyMenu = CreatePopupMenu();
			AppendMenu(NotifyMenu, MF_STRING, IDB_VIEWAPP, L"Open (Ctrl + Space)");
			AppendMenu(NotifyMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(NotifyMenu, MF_STRING, IDB_EXITAPP, L"Exit");
			POINT pos;
			GetCursorPos(&pos);
			TrackPopupMenu(NotifyMenu, TPM_RIGHTBUTTON, pos.x, pos.y, 0, hWnd, NULL);

			break;
		}
		}
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate2);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand2);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy2);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint2);
		HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem2);
	case WM_CLOSE:
	{
		if (!g_Added)
		{
			int chon = MessageBox(hWnd, L"Are you sure to exit without saving this note?", L"Warning", MB_OKCANCEL);
			if (chon == 1)	// thoát
			{
				// clear hết dữ liệu và khung nhập
				SetWindowText(g_tfTitle, L"");
				SetWindowText(g_tfTags, L"");
				SetWindowText(g_tfContent, L"");

				// chuyển màn hình
				EnableWindow(g_HWND1, 1);
				ShowWindow(g_HWND1, 1);
				ShowWindow(g_HWND2, 0);
				g_onWindow = 1;
				g_openFromLV = false;
			}
		}
		break;
	}
	case WM_SETFOCUS:
	{
		SetFocus(g_tfTitle);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate3);
		//HANDLE_MSG(hWnd, WM_COMMAND, OnCommand3);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy3);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint3);
		//HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem3);
	case WM_CLOSE:
	{
		// chuyển màn hình
		EnableWindow(g_HWND1, 1);
		ShowWindow(g_HWND1, 1);
		ShowWindow(g_HWND3, 0);
		g_onWindow = 1;

		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK CatchKeyDown(int message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_SPACE)<0 && GetAsyncKeyState(VK_CONTROL)<0)
	{
		if (IsWindowVisible(g_HWND1) == false)
		{
			ShowWindow(g_HWND1, 1);
			g_onWindow = 1;
		}
		else
		{
			ShowWindow(g_HWND1, 0);
			g_onWindow = 0;
		}
	}
	return CallNextHookEx(g_hook, message, wParam, lParam);

}

// WM cho cửa sổ 1
BOOL OnCreate1(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HIMAGELIST hImageList;
	HICON hicon;
	HBITMAP hbitmap;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, CatchKeyDown, hInst, 0);

	HFONT hFont = CreateFont(20, 0, 0, 100, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Arial");

	g_btnNewNote = CreateWindowW(L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
		900, 1, 125, 35, hwnd, (HMENU)ID_BTN_NEWNOTE, hInst, NULL);

	g_btnStatistics = CreateWindowW(L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
		165, 1, 125, 35, hwnd, (HMENU)ID_BTN_STATISTICS, hInst, NULL);

	g_btnDeleteNote = CreateWindowW(L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
		530, 675, 125, 35, hwnd, (HMENU)ID_BTN_DELETE, hInst, NULL);

	g_tvTags = CreateWindowW(WC_TREEVIEW, _T("Tree View"),WS_VISIBLE | WS_CHILD | WS_VSCROLL | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		0, 48, 288, 616, hwnd, (HMENU)IDC_TREEVIEW, hInst, NULL);
	SetWindowFont(g_tvTags, hFont, true);
	SendMessage(g_tvTags, TVM_SETTEXTCOLOR, 0, COLORREF(RGB(255, 255, 255)));
	SendMessage(g_tvTags, TVM_SETLINECOLOR, 0, COLORREF(RGB(255, 255, 255)));
	SendMessage(g_tvTags, TVM_SETBKCOLOR, 0, COLORREF(RGB(17, 153, 142)));

	g_lvNotes = CreateWindowW(WC_LISTVIEWW, _T("List View"), WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT,
		300, 46, 883, 620, hwnd, (HMENU)IDC_LISTVIEW, hInst, NULL);
	SetWindowFont(g_lvNotes, hFont, true);

	//thêm các bitmap vào list để tạo icon cho Tree View
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hImageList = ImageList_Create(16, 16, ILC_COLOR16, 0, 0);
	ImageList_Add(hImageList, hbitmap, NULL);

	hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	ImageList_AddIcon(hImageList, hicon);

	SendMessage(g_tvTags, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);

	init();

	return true;
}

void OnCommand1(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDB_VIEWAPP:
	{
		ShowWindow(g_HWND1, 1);
		g_onWindow = 1;
		break;
	}
	case IDB_EXITAPP:
	{
		SendMessage(g_HWND1, WM_DESTROY, 0, 0);
		break;
	}
	case ID_BTN_NEWNOTE:
	{
		EnableWindow(g_HWND1, 0);
		ShowWindow(g_HWND2, 1);
		g_onWindow = 2;
		g_Added = false;
		g_openFromLV = false;
		break;
	}
	case ID_BTN_STATISTICS:
	{
		// nếu không có dữ liệu sẽ không tạo thống kê
		if (g_notesList.size() < 1)
		{
			MessageBox(g_HWND1, L"Nothing to show without data!", L"Warning", MB_OK);
			break;
		}
		EnableWindow(g_HWND1, 0);
		ShowWindow(g_HWND3, 1);
		g_onWindow = 3;
		InvalidateRect(g_HWND3, NULL, true);
		break;
	}
	case ID_BTN_DELETE:
	{
		int itemVal = SendMessage(g_lvNotes, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
		LVITEM SelectedItem;
		SelectedItem.iItem = itemVal;

		// kiểm tra người dùng có click vào 1 item hay không
		if (SelectedItem.iItem >= 0)
		{
			ListView_GetItem(g_lvNotes, (LVITEM*)&SelectedItem);
			// lấy id note được chọn
			g_currentIDNote = (int)SelectedItem.lParam;
			int k = MessageBox(g_HWND1, L"Are you sure to delete this note?", L"Warning", MB_OKCANCEL);
			if (k == 1) // đồng ý xóa
			{
				vector<CNote*> tempListNote;
				// xóa note được chọn ra khỏi danh sách note
				for (int i = 0; i < g_notesList.size(); i++)
				{
					if (g_notesList[i]->getID() != g_currentIDNote)
					{
						tempListNote.push_back(g_notesList[i]);
					}
				}
				//for (int i = 0; i < tempListNote.size(); i++)
					//tempListNote[i]->setID(i);
				g_notesList = tempListNote;

				
				// xóa id note đó trong dữ liệu của tag
				for (int i = 0; i < g_tagsList.size(); i++)
				{
					g_tagsList[i]->delNoteID(g_currentIDNote);
				}
				/*for (int i = 0; i < g_tagsList.size(); i++)
				{
					g_tagsList[i]->increaseNoteIDAfterNoteID(g_currentIDNote);
				}*/

				// làm mới listview
				ListView_DeleteAllItems(g_lvNotes);
				for (int i = 0; i < g_notesList.size(); i++)
				{
					AddItemToListView(i, g_notesList[i]->getID(), (WCHAR*)g_notesList[i]->getTitle().c_str(), (WCHAR*)g_notesList[i]->getContent().c_str(), (WCHAR*)g_notesList[i]->getTags().c_str(), (WCHAR*)g_notesList[i]->getDateTime().c_str());
				}

				// xóa các tag không có note
				vector<CTag*> temp;
				for (int i = 0; i < g_tagsList.size(); i++)
				{
					if (g_tagsList[i]->countNote() > 0)
						temp.push_back(g_tagsList[i]);
				}
				g_tagsList = temp;

				// làm mới treeview
				TreeView_DeleteAllItems(g_tvTags);
				g_treeViewRoot = initRootInTV();
				for (int i = 0; i < g_tagsList.size(); i++)
				{
					wstring tagName = g_tagsList[i]->getName();
					int notePerTag = g_tagsList[i]->countNote();
					wstring strNotePerTag = to_wstring(notePerTag);
					tagName += L" (" + strNotePerTag + L")";
					AddItemToTreeView(g_treeViewRoot, (WCHAR*)tagName.c_str());
				}

				InvalidateRect(g_HWND1, NULL, TRUE);
				MessageBeep(MB_OK);
				MessageBox(g_HWND1, L"Note has been removed", L"Warning", MB_OK);
			}
		}
		else
		{
			MessageBox(g_HWND1, L"Please click on note you want to delete!", L"Warning", MB_OK);
		}
	}
	default:
		return;
	}
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
			HTREEITEM selection = (HTREEITEM)TreeView_GetSelection(g_tvTags);
			if (selection == NULL)
				break;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = Text;
			tvItem.cchTextMax = 255;
			tvItem.hItem = selection;

			SendMessage(g_tvTags, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvItem);
			ListView_DeleteAllItems(g_lvNotes);

			wstring tagName = tvItem.pszText;

			// các tag khác gốc sẽ bỏ đi phần đuôi (số lượng note của tag đó)
			if (tagName != L"Tags")
			{
				int len = tagName.length();
				int pos = tagName.find_last_of(L"(");
				tagName.erase(pos - 1, len - pos + 1);
			}
			
			if (tagName == L"Tags")
			{
				// nếu chọn 'Tags' trên cây tree view sẽ đổ ra hết tất cả note lên list view
				for (int i = 0; i < g_notesList.size(); i++)
				{
					AddItemToListView(i, g_notesList[i]->getID(),(WCHAR*)g_notesList[i]->getTitle().c_str(), (WCHAR*)g_notesList[i]->getContent().c_str(), (WCHAR*)g_notesList[i]->getTags().c_str(), (WCHAR*)g_notesList[i]->getDateTime().c_str());
				}
			}
			else
			{
				// nếu chọn 1 mục khác 'Tags' trên cây tree view sẽ đổ ra hết tất cả note có chứa tag đó lên list view
				for (int i = 0,j=0; i < g_notesList.size(); i++)
				{
					wstring tagsInNote = g_notesList[i]->getTags();
					int found = tagsInNote.find(tagName);
					int strLen = tagsInNote.length();
					int subStrLen = tagName.length();

					if (found < 0)
						continue;

					// kiểm tra các khả năng chuỗi nhỏ bị trùng trong 1 chuỗi lớn
					if (found == 0)
					{
						// nếu tìm thấy chuỗi con ở đầu chuỗi mẹ, kí tự liền tiếp phải là dấu ','
						if (tagsInNote[subStrLen] != L',')
							found = -1;
						if(subStrLen == strLen)
							found = 0;
					}
					else if (found == strLen - subStrLen)
					{
						// nếu tìm thấy chuỗi con ở cuối chuỗi mẹ, kí tự liền trước phải là dấu ' '
						if (tagsInNote[found - 1] != L' ')
							found = -1;
					}
					else
					{
						// nếu tìm thấy chuỗi con ở giữa chuỗi mẹ, kí tự liền tiếp phải là dấu ',' và liền trước phải là ' '
						if (tagsInNote[found - 1] != L' ' || tagsInNote[found + subStrLen] != L',')
							found = -1;
					}

					if (found >= 0)
					{
						AddItemToListView(j, g_notesList[i]->getID(), (WCHAR*)g_notesList[i]->getTitle().c_str(), (WCHAR*)g_notesList[i]->getContent().c_str(), (WCHAR*)g_notesList[i]->getTags().c_str(), (WCHAR*)g_notesList[i]->getDateTime().c_str());
						j++;
					}
				}
			}
		}
		break;
	case IDC_LISTVIEW:
		if (pnm->code == NM_DBLCLK)
		{
			int itemVal = SendMessage(g_lvNotes, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			LVITEM SelectedItem;
			SelectedItem.iItem = itemVal;
			
			// kiểm tra người dùng có click vào 1 item hay không
			if (SelectedItem.iItem >= 0)
			{
				ListView_GetItem(g_lvNotes, (LVITEM*)&SelectedItem);
				// lấy id note được chọn
				g_currentIDNote = (int)SelectedItem.lParam;
				
				EnableWindow(g_HWND1, 0);
				ShowWindow(g_HWND2, 1);
				g_onWindow = 2;
				g_Added = false;
				g_openFromLV = true;

				for (int i = 0; i < g_notesList.size(); i++)
				{
					if (g_notesList[i]->getID() == g_currentIDNote)
						g_currentIDNote = i;
				}

				SetWindowText(g_tfTitle, g_notesList[g_currentIDNote]->getTitle().c_str());
				SetWindowText(g_tfTags, g_notesList[g_currentIDNote]->getTags().c_str());
				SetWindowText(g_tfContent, g_notesList[g_currentIDNote]->getContent().c_str());
			}
		}
		break;
	}
	return 0;
}

void OnPaint1(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	Graphics *graphics = new Graphics(hdc);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// tạo bút vẽ, màu vẽ
	Pen whitePen(WHITE);
	Rect rect(-10, 0, 1210, 750);
	SolidBrush sb(WHITE);
	LinearGradientBrush lgb(rect, GREENL, GREENR, LinearGradientModeHorizontal);
	
	// vẽ nền
	graphics->FillRectangle(&lgb, rect);
	graphics->DrawRectangle(&whitePen, 0, 46, 290, 620);
	graphics->DrawRectangle(&whitePen, 0, 47, 289, 618);
	graphics->FillEllipse(&sb, 148, 0, 35, 35);
	graphics->FillEllipse(&sb, 273, 0, 35, 35);
	graphics->FillEllipse(&sb, 883, 0, 35, 35);
	graphics->FillEllipse(&sb, 1008, 0, 35, 35);
	graphics->FillEllipse(&SolidBrush(RED), 512, 675, 35, 35);
	graphics->FillEllipse(&SolidBrush(RED), 638, 675, 35, 35);
	
	//530, 675, 125, 35

	EndPaint(hwnd, &ps);
}

void OnDrawItem1(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
	switch (lpDrawItem->CtlID)
	{
	case ID_BTN_NEWNOTE:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(WHITE);
		graphics->FillRectangle(&brush, 0, 0, 500, 500);

		// vẽ icon
		auto image = new Image(L"resource\\newnote.png");
		graphics->DrawImage(image, 8, 3, 25, 25);

		// vẽ chữ
		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18,FontStyleRegular, UnitPixel);
		PointF      pointF(35, 6);
		SolidBrush  solidBrush(GREENR);
		graphics->DrawString(L"New note", -1, &font, pointF, &solidBrush);

		if (image)
			delete image;
		if (graphics)
			delete graphics;
		break;
	}
	case ID_BTN_STATISTICS:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(WHITE);
		graphics->FillRectangle(&brush, 0, 0, 500, 500);

		// vẽ icon
		auto image = new Image(L"resource\\statistics.png");
		graphics->DrawImage(image, 8, 5, 25, 25);

		// vẽ chữ
		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18, FontStyleRegular, UnitPixel);
		PointF      pointF(37, 6);
		SolidBrush  solidBrush(GREENL);
		graphics->DrawString(L"Statistics", -1, &font, pointF, &solidBrush);

		if (image)
			delete image;
		if (graphics)
			delete graphics;
		break;
	}
	case ID_BTN_DELETE:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(RED);
		graphics->FillRectangle(&brush, 0, 0, 500, 500);

		// vẽ icon
		auto image = new Image(L"resource\\delete.png");
		graphics->DrawImage(image, 8, 5, 25, 25);

		// vẽ chữ
		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18, FontStyleRegular, UnitPixel);
		PointF      pointF(37, 6);
		SolidBrush  solidBrush(WHITE);
		graphics->DrawString(L"DELETE", -1, &font, pointF, &solidBrush);

		if (image)
			delete image;
		if (graphics)
			delete graphics;
		break;
	}
	}
}

void OnDestroy1(HWND hwnd)
{
	GdiplusShutdown(gdiplusToken);
	SaveTagToFile();
	SaveNoteToFile();
	PostQuitMessage(0);
}




// WM cho cửa sổ 2
BOOL OnCreate2(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HFONT hFont = CreateFont(22, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Arial");


	// nút Save
	g_btnSaveNote = CreateWindowW(L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
								350, 1, 85, 35, hwnd, (HMENU)ID_BTN_SAVENOTE, hInst, NULL);

	// khung nhập tiêu đề cho note
	g_tfTitle = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, 
								160, 62, 525, 26, hwnd, (HMENU)ID_EDIT_TITLE, hInst, NULL);
	SetWindowFont(g_tfTitle, hFont, true);

	// khung nhập thẻ cho note
	g_tfTags = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
		160, 125, 525, 26, hwnd, (HMENU)ID_EDIT_TAGS, hInst, NULL);
	SetWindowFont(g_tfTags, hFont, true);

	// khung nhập nội dung cho note
	g_tfContent = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP,
								160, 185, 535, 300, hwnd, NULL, hInst, NULL);
	SetWindowFont(g_tfContent, hFont, true);

	return true;
}

void OnCommand2(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_BTN_SAVENOTE:
	{
		// lấy chiều dài các chuỗi trong 3 ô nhập
		int len1 = GetWindowTextLength(g_tfTitle);
		int len2 = GetWindowTextLength(g_tfTags);
		int len3 = GetWindowTextLength(g_tfContent);

		// báo lỗi khi ô tiêu đề bỏ trống
		if (len1 == 0) {
			MessageBeep(MB_ICONERROR);
			MessageBox(hwnd, L"Title box is empty!", L"Warning", MB_OK);
			SetFocus(g_tfTitle);
			break;
		}
		// báo lỗi khi ô thẻ bỏ trống
		if (len2 == 0) {
			MessageBeep(MB_ICONERROR);
			MessageBox(hwnd, L"Tags box is empty!", L"Warning", MB_OK);
			SetFocus(g_tfTags);
			break;
		}
		// báo lỗi khi ô nội dung bỏ trống
		if (len3 == 0) {
			MessageBeep(MB_ICONERROR);
			MessageBox(hwnd, L"Content box is empty!", L"Warning", MB_OK);
			SetFocus(g_tfContent);
			break;
		}

		WCHAR *buffTitle = new WCHAR[len1 + 1];
		WCHAR *buffTags = new WCHAR[len2 + 1];
		WCHAR *buffContent = new WCHAR[len3 + 1];

		// lấy chuỗi trong 3 ô nhập
		GetWindowText(g_tfTitle, buffTitle, len1 + 1);
		GetWindowText(g_tfTags, buffTags, len2 + 1);
		GetWindowText(g_tfContent, buffContent, len3 + 1);

		// ép kiểu về wstring
		wstring strTitle = wstring(buffTitle);
		wstring strTags = wstring(buffTags);

		// chuẩn hóa chuỗi do người dùng nhập (chuỗi nội dung thì không cần chuẩn hóa)
		StandardizeString(strTitle);
		StandardizeString(strTags);
		delRedunComma(strTags);			// xóa dấu phẩy thừa trong chuỗi Tag

		vector<wstring> tagNameList;		// vector chứa các chuỗi tag
		tagNameList = getTagList(strTags);	// tách chuỗi tag nhập vào thành các tag

		strTags = L"";
		int nTags = tagNameList.size();
		for (int i = 0; i < nTags; i++)
		{
			strTags += tagNameList[i];
			if (i < nTags-1)
				strTags += L", ";
		}

		wstring time = getCurrentDateTime();

		// thêm note vào g_noteList nếu được mở từ nút new note của cửa sổ chính
		if (!g_openFromLV)
		{
			// thêm note thành công, tăng số lượng note
			g_noteID++;
			CNote *note = new CNote(g_noteID, strTitle, (wstring)buffContent, strTags, time);
			g_notesList.push_back(note);
		}
		else	// sửa lại note của g_noteList dựa vào ID có sẵn khi được mở từ listview
		{
			g_notesList[g_currentIDNote]->setTitle(strTitle);
			g_notesList[g_currentIDNote]->setTags(strTags);
			g_notesList[g_currentIDNote]->setContent((wstring)buffContent);
			g_notesList[g_currentIDNote]->setDateTime(time);
			for (int i = 0; i < g_tagsList.size(); i++)
			{
				g_tagsList[i]->delNoteID(g_currentIDNote);
			}
		}
		
		ListView_DeleteAllItems(g_lvNotes);
		// thêm note vào list view
		for (int i = 0; i < g_notesList.size(); i++)
		{
			AddItemToListView(i, g_notesList[i]->getID(), (WCHAR*)g_notesList[i]->getTitle().c_str(), (WCHAR*)g_notesList[i]->getContent().c_str(), (WCHAR*)g_notesList[i]->getTags().c_str(), (WCHAR*)g_notesList[i]->getDateTime().c_str());
		}

		bool isTagExist = false;
		int existPos;
		for (int i = 0; i < tagNameList.size(); i++)		// duyệt tất cả các thẻ mới nhập
		{
			for(int j = 0;j<g_tagsList.size();j++)
				if (tagNameList[i] == g_tagsList[j]->getName())
				{
					isTagExist = true;
					existPos = j;
					break;
				}
			if (!isTagExist)	// tag đó chưa tồn tại thì thêm vào tagList
			{
				CTag *tag;
				if (!g_openFromLV)
					tag = new CTag(tagNameList[i], { g_noteID });
				else
					tag = new CTag(tagNameList[i], { g_currentIDNote });
				g_tagsList.push_back(tag);

			}
			else				// tag đó đã tồn tại thì chỉ tăng số lượng note của tag đó
			{
				if (!g_openFromLV)
					g_tagsList[existPos]->addNoteIdToTag(g_noteID);
				else
					g_tagsList[existPos]->addNoteIdToTag(g_currentIDNote);
			}
			isTagExist = false;
		}

		// xóa các tag không có note
		vector<CTag*> temp;
		for (int i = 0; i < g_tagsList.size(); i++)
		{
			if (g_tagsList[i]->countNote() > 0)
				temp.push_back(g_tagsList[i]);
		}
		g_tagsList = temp;

		// load lại cây tree view
		TreeView_DeleteAllItems(g_tvTags);
		g_treeViewRoot = initRootInTV();
		for (int i = 0; i < g_tagsList.size(); i++)
		{
			wstring tagName = g_tagsList[i]->getName();
			int notePerTag = g_tagsList[i]->countNote();
			wstring strNotePerTag = to_wstring(notePerTag);
			tagName += L" (" + strNotePerTag + L")";
			AddItemToTreeView(g_treeViewRoot, (WCHAR*)tagName.c_str());
		}
		
		// clear hết dữ liệu và khung nhập
		SetWindowText(g_tfTitle, L"");
		SetWindowText(g_tfTags, L"");
		SetWindowText(g_tfContent, L"");

		// chuyển màn hình
		EnableWindow(g_HWND1, 1);
		ShowWindow(g_HWND1, 1);
		ShowWindow(g_HWND2, 0);
		g_onWindow = 1;

		g_Added = true;
		InvalidateRect(g_HWND1, NULL, TRUE);
		MessageBeep(MB_OK);
		if(!g_openFromLV)
			MessageBox(g_HWND1, L"Add note completed!", L"Message", MB_OK);
		else
			MessageBox(g_HWND1, L"Save note completed!", L"Message", MB_OK);
		g_openFromLV = false;
		break;
	}
	default:
		return;
	}
}

void OnPaint2(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	Graphics *graphics = new Graphics(hdc);
	SolidBrush sbW(WHITE);
	SolidBrush sbR(RED);
	SolidBrush sbTrans(TRANS);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// tạo bút vẽ, màu vẽ
	Pen pen(Color(182, 237, 201));
	Rect rect(-10, 0, 810, 550);
	LinearGradientBrush lgb(rect, GREENL, GREENR, LinearGradientModeHorizontal);

	// tô nền
	graphics->FillRectangle(&lgb, rect);

	graphics->FillRectangle(&sbTrans, 88, 48, 610, 50);
	graphics->FillPie(&sbTrans, RectF(63, 48, 50, 50), 90, 180);
	graphics->FillPie(&sbTrans, RectF(673, 48, 50, 50), -90, 180);
	graphics->FillRectangle(&sbW, 158, 58, 540, 30);
	graphics->FillPie(&sbW, RectF(682, 58, 30, 30), -90, 180);

	graphics->FillRectangle(&sbTrans, 88, 111, 610, 50);
	graphics->FillPie(&sbTrans, RectF(63, 111, 50, 50), 90, 180);
	graphics->FillPie(&sbTrans, RectF(673, 111, 50, 50), -90, 180);
	graphics->FillRectangle(&sbW, 158, 121, 540, 30);
	graphics->FillPie(&sbW, RectF(682, 121, 30, 30), -90, 180);


	graphics->FillRectangle(&sbTrans, 88, 174, 610, 25);
	graphics->FillPie(&sbTrans, RectF(63, 174, 50, 50), -180, 90);
	graphics->FillPie(&sbTrans, RectF(673, 174, 50, 50), -90, 90);
	graphics->FillRectangle(&sbTrans, 63, 199, 660, 271);
	graphics->FillRectangle(&sbTrans, 88, 470, 610, 25);
	graphics->FillPie(&sbTrans, RectF(63, 445, 50, 50), 90, 90);
	graphics->FillPie(&sbTrans, RectF(673, 445, 50, 50), 0, 90);
	graphics->FillPie(&sbW, RectF(670, 184, 40, 50), 0, 360);
	graphics->FillRectangle(&sbW, 670, 209, 40, 250);
	graphics->FillPie(&sbW, RectF(670, 435, 40, 50), 0, 360);

	graphics->FillEllipse(&sbW, 333, 0, 35, 35);
	graphics->FillEllipse(&sbW, 418, 0, 35, 35);

	// vẽ 3 nhãn
	FontFamily  fontFamily(L"Arial");
	Font        font(&fontFamily, 20, FontStyleBold, UnitPixel);
	SolidBrush  sb3(Color(237, 138, 33));

	PointF pointF(100, 60);
	graphics->DrawString(L"Title:", -1, &font, pointF, &sb3);

	SolidBrush  sb4(Color(14, 183, 68));
	pointF = PointF(96, 123);
	graphics->DrawString(L"Tags:", -1, &font, pointF, &sb4);
	
	SolidBrush  sb5(Color(27, 191, 193));
	pointF = PointF(70, 300);
	graphics->DrawString(L"Content:", -1, &font, pointF, &sb5);

	EndPaint(hwnd, &ps);
}

void OnDrawItem2(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
	switch (lpDrawItem->CtlID)
	{
	case ID_BTN_SAVENOTE:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(WHITE);
		graphics->FillRectangle(&brush, 0, 0, 500, 500);

		// vẽ icon
		auto image = new Image(L"resource\\save.png");
		graphics->DrawImage(image, 5, 5, 25, 25);

		// vẽ chữ
		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18, FontStyleRegular, UnitPixel);
		PointF      pointF(33, 6);
		SolidBrush  solidBrush(GREENR);
		graphics->DrawString(L"Save", -1, &font, pointF, &solidBrush);

		if (image)
			delete image;
		if (graphics)
			delete graphics;
		break;
	}
	default:
		break;
	}
}

void OnDestroy2(HWND hwnd)
{
}




// WM cho cửa sổ 3
BOOL OnCreate3(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HFONT hFont = CreateFont(22, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Arial");


	return true;
}

void OnPaint3(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	REAL start[5] = { 0.0 };
	REAL sweep[5] = { 0.0 };
	start[0] = -90;
	Graphics *graphics = new Graphics(hdc);
	SolidBrush sbW(WHITE);
	SolidBrush sbR(RED);
	SolidBrush sbTrans(TRANS);
	SolidBrush sbBlueGrey(BLUEGREY);
	SolidBrush  sbR1(RANK1CLR);
	SolidBrush  sbR2(RANK2CLR);
	SolidBrush  sbR3(RANK3CLR);
	SolidBrush  sbR4(RANK4CLR);
	SolidBrush  sbR5(RANK5CLR);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);
	
	// lấy ra tối đa 5 tag nhiều note nhất
	vector<CTag*> topTags = getTopFiveTag();

	// lấy ra tổng số tag
	int nTags = 0;
	int nTagsLeft;
	for (int i = 0; i < g_tagsList.size(); i++)
	{
		nTags += g_tagsList[i]->countNote();
	}

	// tạo bút vẽ, màu vẽ
	Pen pen(Color(182, 237, 201));
	Rect rect(-10, 0, 610, 650);
	LinearGradientBrush lgb(rect, GREENL, GREENR, LinearGradientModeHorizontal);

	// tô nền
	graphics->FillRectangle(&lgb, rect);
	graphics->FillRectangle(&sbTrans, -5, 175, 100, 40);
	graphics->FillPie(&sbTrans, RectF(75, 175, 40, 40), -90, 180);

	// tô phần header
	graphics->FillRectangle(&sbTrans, 110, 10, 360, 50);
	graphics->FillPie(&sbTrans, RectF(85, 10, 50, 50), 90, 180);
	graphics->FillPie(&sbTrans, RectF(445, 10, 50, 50), -90, 180);


	// tô phần biểu đồ tròn
	graphics->FillEllipse(&sbTrans, 155, 65, 270, 270);
	graphics->FillEllipse(&sbW, 165, 75, 250, 250);
	nTagsLeft = nTags;
	for (int i = 0; i < topTags.size(); i++) {
		nTagsLeft -= topTags[i]->countNote();
	}

	// tô phần chú thích
	graphics->FillRectangle(&sbTrans, 40, 340, 502, 25);
	graphics->FillPie(&sbTrans, RectF(15, 340, 50, 50), -180, 90);
	graphics->FillPie(&sbTrans, RectF(517, 340, 50, 50), -90, 90);
	graphics->FillRectangle(&sbTrans, 15, 365, 552, 260);


	// vẽ chữ và icon
	FontFamily  fontFamily(L"Arial");
	Font        font(&fontFamily, 26, FontStyleBold, UnitPixel);

	PointF pointF(96, 20);
	graphics->DrawString(L"Top 5 tags with highest appear", -1, &font, pointF, &sbW);

	wstring strText, count;

	// ------------- hạng 1
	sweep[0] = (topTags[0]->countNote() / (nTags * 1.0)) * 360;
	graphics->FillPie(&sbR1, RectF(165, 75, 250, 250), start[0], sweep[0]);
	auto image = new Image(L"resource\\rank1.png");
	graphics->DrawImage(image, 120, 350, 52, 52);
	
	pointF = PointF(180, 360);
	count = to_wstring(topTags[0]->countNote());
	strText = topTags[0]->getName();
	if (strText.length() > 16)
	{
		strText.erase(15, strText.length() - 15);
		strText += L"...";
	}
	strText = strText + L" (" + count + L")";
	graphics->DrawString(strText.c_str(), -1, &font, pointF, &sbR1);


	// ------------- hạng 2 nếu có
	if (topTags.size() >= 2)
	{
		start[1] = start[0] + sweep[0];
		sweep[1] = (topTags[1]->countNote() / (nTags * 1.0)) * 360;
		graphics->FillPie(&sbR2, RectF(165, 75, 250, 250), start[1], sweep[1]);
		image = new Image(L"resource\\rank2.png");
		graphics->DrawImage(image, 125, 417, 42, 42);
		pointF = PointF(181, 423);
		Font        font2(&fontFamily, 22, FontStyleBold, UnitPixel);
		count = to_wstring(topTags[1]->countNote());
		strText = topTags[1]->getName();
		if (strText.length() > 25)
		{
			strText.erase(24, strText.length() - 24);
			strText += L"...";
		}
		strText = strText + L" (" + count + L")";
		graphics->DrawString(strText.c_str(), -1, &font2, pointF, &sbR2);
	}


	// ------------- hạng 3 nếu có
	if (topTags.size() >= 3)
	{
		start[2] = start[1] + sweep[1];
		sweep[2] = (topTags[2]->countNote() / (nTags * 1.0)) * 360;
		graphics->FillPie(&sbR3, RectF(165, 75, 250, 250), start[2], sweep[2]);
		image = new Image(L"resource\\rank3.png");
		graphics->DrawImage(image, 128, 474, 36, 36);
		pointF = PointF(182, 482);
		Font        font3(&fontFamily, 18, FontStyleBold, UnitPixel);
		count = to_wstring(topTags[2]->countNote());
		strText = topTags[2]->getName();
		if (strText.length() > 31)
		{
			strText.erase(30, strText.length() - 30);
			strText += L"...";
		}
		strText = strText + L" (" + count + L")";
		graphics->DrawString(strText.c_str(), -1, &font3, pointF, &sbR3);
	}
	

	// ------------- hạng 4 nếu có
	Font        font45(&fontFamily, 16, FontStyleBold, UnitPixel);
	if (topTags.size() >= 4)
	{
		start[3] = start[2] + sweep[2];
		sweep[3] = (topTags[3]->countNote() / (nTags * 1.0)) * 360;
		graphics->FillPie(&sbR4, RectF(165, 75, 250, 250), start[3], sweep[3]);
		image = new Image(L"resource\\rank4.png");
		graphics->DrawImage(image, 130, 525, 32, 32);
		pointF = PointF(183, 530);
		count = to_wstring(topTags[3]->countNote());
		strText = topTags[3]->getName();
		if (strText.length() > 33)
		{
			strText.erase(32, strText.length() - 32);
			strText += L"...";
		}
		strText = strText + L" (" + count + L")";
		graphics->DrawString(strText.c_str(), -1, &font45, pointF, &sbR4);
	}


	// ------------- hạng 5 nếu có
	if (topTags.size() >= 5)
	{
		start[4] = start[3] + sweep[3];
		sweep[4] = (topTags[4]->countNote() / (nTags * 1.0)) * 360;
		graphics->FillPie(&sbR5, RectF(165, 75, 250, 250), start[4], sweep[4]);
		image = new Image(L"resource\\rank5.png");
		graphics->DrawImage(image, 130, 570, 32, 32);
		pointF = PointF(183, 575);
		count = to_wstring(topTags[4]->countNote());
		strText = topTags[4]->getName();
		if (strText.length() > 33)
		{
			strText.erase(32, strText.length() - 32);
			strText += L"...";
		}
		strText = strText + L" (" + count + L")";
		graphics->DrawString(strText.c_str(), -1, &font45, pointF, &sbR5);
	}

	graphics->FillEllipse(&sbBlueGrey, 230, 140, 120, 120);
	Font        fontSum(&fontFamily, 66, FontStyleBold, UnitPixel);
	if(nTags < 10)
		pointF = PointF(265, 160);
	else
		pointF = PointF(244, 160);
	graphics->DrawString((to_wstring(nTags)).c_str(), -1, &fontSum, pointF, &SolidBrush(GREENR));

	graphics->FillPie(&sbW, RectF(-10, 180, 50, 50), -60, 60);
	Font        fontS(&fontFamily, 15, FontStyleBold, UnitPixel);
	pointF = PointF(45, 190);
	graphics->DrawString(L"Another", -1, &fontS, pointF, &sbW);

	EndPaint(hwnd, &ps);
}

void OnDestroy3(HWND hwnd)
{
}





// ------------------------------------------- hàm hỗ trợ -------------------------------------------

// khởi tạo những thứ cần thiết
void init()
{
	g_treeViewRoot = initRootInTV();
	InitListViewInterface();
	LoadTagFromFile();
	LoadNoteFromFile();
}

// tạo root cho treeview tags
HTREEITEM initRootInTV()
{
	TVINSERTSTRUCT rootStruct;
	rootStruct.hParent = TVI_ROOT;
	rootStruct.hInsertAfter = TVI_FIRST;
	rootStruct.item.iImage = 0;
	rootStruct.item.iSelectedImage = 0;
	rootStruct.item.cchTextMax = 255;
	rootStruct.item.lParam = (LPARAM)L"Tags";
	rootStruct.item.pszText = (WCHAR*)L"Tags";
	rootStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	return (HTREEITEM)SendMessage(g_tvTags, TVM_INSERTITEM, 0, (LPARAM)&rootStruct);
}

// thêm 1 tag vào treeview
void AddItemToTreeView(HTREEITEM root, WCHAR *tagName)
{
	TVINSERTSTRUCT nodeStruct;
	nodeStruct.hParent = root;
	nodeStruct.hInsertAfter = root;
	nodeStruct.item.iImage = 0;
	nodeStruct.item.iSelectedImage = 0;
	nodeStruct.item.cchTextMax = 255;
	nodeStruct.item.lParam = (LPARAM)tagName;
	nodeStruct.item.pszText = tagName;
	nodeStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	SendMessage(g_tvTags, TVM_INSERTITEM, 0, (LPARAM)&nodeStruct);
}

// Khởi tạo giao diện List View (cột, kích thước,...)
void InitListViewInterface()
{
	LVCOLUMN lvColumn[4];

	lvColumn[0].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[0].fmt = LVCFMT_LEFT;
	lvColumn[0].cx = 200;
	lvColumn[0].pszText = (WCHAR*)L"Title";
	ListView_InsertColumn(g_lvNotes, 0, &lvColumn[0]);

	lvColumn[1].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[1].fmt = LVCFMT_LEFT;
	lvColumn[1].cx = 300;
	lvColumn[1].pszText = (WCHAR*)L"Content";
	ListView_InsertColumn(g_lvNotes, 1, &lvColumn[1]);

	lvColumn[2].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[2].fmt = LVCFMT_LEFT;
	lvColumn[2].cx = 220;
	lvColumn[2].pszText = (WCHAR*)L"Tags";
	ListView_InsertColumn(g_lvNotes, 2, &lvColumn[2]);

	lvColumn[3].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[3].fmt = LVCFMT_RIGHT;
	lvColumn[3].cx = 160;
	lvColumn[3].pszText = (WCHAR*)L"Last modified";
	ListView_InsertColumn(g_lvNotes, 3, &lvColumn[3]);

	ListView_SetTextColor(g_lvNotes, RGB(0, 135, 72));
	ListView_SetExtendedListViewStyle(g_lvNotes, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

// Thêm 1 item vào List View
void AddItemToListView(int i, int idNote, WCHAR* title, WCHAR* content, WCHAR* tag, WCHAR* date)
{
	
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = title;
	lvItem.lParam = (LPARAM)idNote;
	lvItem.cchTextMax = 255;
	lvItem.iItem = i;
	ListView_InsertItem(g_lvNotes, &lvItem);
	ListView_SetItemText(g_lvNotes, i, 0, title);
	ListView_SetItemText(g_lvNotes, i, 1, content);
	ListView_SetItemText(g_lvNotes, i, 2, tag);
	ListView_SetItemText(g_lvNotes, i, 3, date);
}

// Chuẩn hóa chuỗi (xóa khoảng trắng thừa)
void StandardizeString(wstring &str)
{
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ' ' && i == 0)						// kí tự đầu chuỗi là ' ' thì xóa
		{
			str.erase(0, 1);
			i--;
		}
		else if (str[i] == ' ' && str[i + 1] == ' ')		// kí tự thứ i và sau i là ' ' thì xóa 1 ' '
		{
			str.erase(i, 1);
			i--;
		}
		else if (str[i] == ' ' && i == str.length() - 1)	// kí tự cuối chuỗi là ' ' thì xóa
		{													// rồi break thoát luôn hàm.
			str.erase(i, 1);
			break;
		}
		else {}
	}
}

// Xóa dấu ',' thừa trong 1 chuỗi
void delRedunComma(wstring &str)
{
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ',' && i == 0)
		{
			str.erase(i, 1);
			i--;
		}
		else if (str[i] == ' ' && str[i + 1] == ',' && i == 0)
		{
			str.erase(i, 1);
			i--;
		}
		else if (str[i] == ',' && str[i + 1] == ',')
		{
			str.erase(i, 1);
			i--;
		}
		else if (str[i] == ',' && str[i + 1] == ' ' && str[i + 2] == ',')
		{
			str.erase(i, 2);
			i -= 2;
		}
		else if (str[i] == ',' && i == str.length() - 1)
		{
			str.erase(i, 1);
			break;
		}
		else {}
	}
}

// Hàm lấy danh sách các tag từ 1 chuỗi (các tag phân cách bằng dấu ',', trường hợp chuỗi có chuỗi con 
// chỉ có các kí tự ',' liên tiếp nhau thì chuỗi con đó sẽ không tính là 1 tag)
vector<wstring> getTagList(wstring str)
{
	vector<wstring> res;
	int exist = 0;

	int len = str.length();
	vector<int> commaPos;

	for (int i = 0; i < len; i++)			// lưu vị trí các dấu phẩy
		if (str[i] == 44)
			commaPos.push_back(i);

	if (commaPos.size() == 0)				// chỉ có 1 tag thì thêm vào list rồi thoát
	{
		res.push_back(str);
		return res;
	}

	int nTags = commaPos.size() + 1;		// thêm 1 tag cuối

	wstring temp;
	int startPos = 0;
	int endPos = commaPos[0];

	for (int i = 1; i <= nTags; i++)		// tiến hành lấy các tag để thêm vào danh sách
	{
		exist = 0;
		temp = str.substr(startPos, endPos - startPos);
		StandardizeString(temp);

		for (int j = 0; j < res.size(); j++)
		{
			if (res[j] == temp) // chuỗi có tag bị lặp
			{
				exist = 1;
				break;
			}
		}
		if (exist != 1)
			res.push_back(temp);
		startPos = endPos + 1;
		if (i < nTags - 1)
			endPos = commaPos[i];
		else
			endPos = len;
	}

	return res;
}

// Lưu dữ liệu tag lên file
void SaveTagToFile() 
{
	wofstream fout;
	vector<int> idList;

	fout.open(g_TagFile);
	fout.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));
	for (int i = 0; i < g_tagsList.size(); i++) {
		fout << g_tagsList[i]->getName() << L"\n";
		idList = g_tagsList[i]->getNoteIDs();
		fout << to_wstring(g_tagsList[i]->countNote()) << L"\n";
		for (int j = 0;j < g_tagsList[i]->countNote(); j++)
		{
			fout << to_wstring(idList[j]) << L" ";
		}
		fout << L"\n\n";
	}
	fout.close();
}

// Đổ dữ liệu từ file tag
void LoadTagFromFile()
{
	wifstream fin;
	wstring wTagName, wCount;
	vector<int> notesID;
	int id = 0, nNote = 0;

	// mở file data để đọc
	fin.open(g_TagFile);
	fin.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

	// đọc dữ liệu
	while (!fin.eof())
	{
		notesID.clear();
		// đọc tên tag
		getline(fin, wTagName);

		// đọc số lượng note của tag
		getline(fin, wCount);
		nNote = _wtoi(wCount.c_str());

		// đọc từng id của note
		for (int i = 0; i < nNote; i++)
		{
			fin >> id;
			notesID.push_back(id);
			fin.ignore();
		}
		
		// tới cuối file, thoát.
		if (wTagName == L"" || wCount == L"" || nNote == 0)
			break;


		// tạo mới 1 tag
		CTag *tag = new CTag(wTagName, notesID);
		// thêm tag vào danh sách tag
		g_tagsList.push_back(tag);

		// bỏ dòng trống
		fin.ignore();
		fin.ignore();
	}

	// nếu file có dữ liệu thì sẽ load lên tree view
	if (!g_tagsList.empty())
	{
		TreeView_DeleteAllItems(g_tvTags);
		g_treeViewRoot = initRootInTV();
		for (int i = 0; i < g_tagsList.size(); i++)
		{
			wstring tagName = g_tagsList[i]->getName();
			int notePerTag = g_tagsList[i]->countNote();
			wstring strNotePerTag = to_wstring(notePerTag);
			tagName += L" (" + strNotePerTag + L")";
			AddItemToTreeView(g_treeViewRoot, (WCHAR*)tagName.c_str());
		}
	}

	// đóng file
	fin.close();
}

// Lưu dữ liệu note lên file
void SaveNoteToFile()
{
	wofstream fout;

	fout.open(g_NoteFile);
	fout.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));
	for (int i = 0; i < g_notesList.size(); i++) {
		fout << to_wstring(i) << L"\n";
		fout << g_notesList[i]->getTitle() << L"\n";
		fout << g_notesList[i]->getTags() << L"\n";
		fout << g_notesList[i]->getContent() << L"\n";
		fout << g_notesList[i]->getDateTime() << L"\n\n";
	}
	fout.close();
}

// Đổ dữ liệu từ file note
void LoadNoteFromFile()
{
	wifstream fin;
	wstring wID, wTitle, wTags, wContent, wTime;
	int id = 0;

	// mở file data để đọc
	fin.open(g_NoteFile);
	fin.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

	// đọc dữ liệu
	while (!fin.eof())
	{
		// đọc id của note
		getline(fin, wID);
		id = _wtoi(wID.c_str());

		// đọc tên note
		getline(fin, wTitle);

		// đọc thẻ của note
		getline(fin, wTags);

		// đọc nội dung note
		wContent = L"";
		wstring temp = L"";
		while (true)
		{
			getline(fin, temp);
			wContent += temp;
			int len = temp.length();
			if (len < 1)
				break;
			if (temp[len - 1] != L'\r')
				break;
			wContent += L'\n';
		}

		// đọc thời gian chỉnh sửa note
		getline(fin, wTime);

		// tới cuối file, thoát.
		if (wID == L"" || wTitle == L"" || wTags == L"" || wContent == L"" || wTime == L"")
			break;

		// tạo mới 1 note
		CNote *note = new CNote(id, wTitle, wContent, wTags,wTime);
		// thêm vào danh sách note
		g_notesList.push_back(note);

		// bỏ dòng trống
		fin.ignore();
	}

	// nếu file có dữ liệu thì sẽ load lên list view
	if (!g_notesList.empty())
	{
		for (int i = 0; i < g_notesList.size(); i++)
		{
			AddItemToListView(i,g_noteID, (WCHAR*)g_notesList[i]->getTitle().c_str(), (WCHAR*)g_notesList[i]->getContent().c_str(), (WCHAR*)g_notesList[i]->getTags().c_str(), (WCHAR*)g_notesList[i]->getDateTime().c_str());
			g_noteID++;
		}
	}
	g_noteID--;
	// đóng file
	fin.close();
}

// thêm chương trình vào notification area
BOOL AddNotificationIcon(HWND hwnd, HINSTANCE hInstance)
{
	NOTIFYICONDATA nid = { 0 };

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP;
	nid.uCallbackMessage = ID_NOTI_AREA;

	LoadIconMetric(hInstance, MAKEINTRESOURCE(IDI_MY1512605QUICKNOTE), LIM_SMALL, &nid.hIcon);
	lstrcpy(nid.szTip, L"Quick Note");
	Shell_NotifyIcon(NIM_ADD, &nid);

	// NOTIFYICON_VERSION_4 is prefered
	nid.uVersion = NOTIFYICON_VERSION_4;
	return (Shell_NotifyIcon(NIM_SETVERSION, &nid) != 0);
}

// lấy ngày giờ hiện tại của hệ thống
wstring getCurrentDateTime()
{
	wstring res;

	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);

	wstring year = to_wstring(now->tm_year + 1900);
	wstring month = to_wstring(now->tm_mon + 1);
	wstring day = to_wstring(now->tm_mday);
	wstring hour = to_wstring(now->tm_hour);
	wstring min = to_wstring(now->tm_min);

	if (min.length() == 1)
		min = L"0" + min;
	if (hour.length() == 1)
		hour = L"0" + hour;

	res = hour + L":" + min + L" - " + day + L"/" + month + L"/" + year;

	return res;
}

// lấy ra tối đa 5 tag nhiều note nhất nếu có
vector<CTag*> getTopFiveTag()
{
	vector<CTag*> tempList = g_tagsList;
	CTag* temp = new CTag();
	int nTags = g_tagsList.size();

	for (int i = 0; i < nTags - 1; i++)
	{
		for (int j = i + 1; j < nTags; j++)
		{
			if (tempList[i]->countNote() < tempList[j]->countNote())
			{
				CTag *temp = tempList[i];
				tempList[i] = tempList[j];
				tempList[j] = temp;
			}
		}
	}

	vector<CTag*> res;

	for (int i = 0; i < 5 && i < tempList.size(); i++)
	{
		res.push_back(tempList[i]);
	}
	
	return res;
}
