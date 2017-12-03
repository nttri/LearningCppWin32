// 1512605_QuanLyChiTieu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512605_QuanLyChiTieu.h"
#include "Pie.h"

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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512605_QUANLYCHITIEU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512605_QUANLYCHITIEU));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512605_QUANLYCHITIEU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512605_QUANLYCHITIEU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 1000, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// VARIABLES
ULONG_PTR	token;
GdiplusStartupInput gdiplusStartupInput;

HWND g_TypeComboBox, g_ContentBox, g_CostBox, g_AddButton, g_ListBox, g_SumBox,
g_RedTitle, g_OrangeTitle, g_YellowTitle, g_GreenTitle, g_Blue1Title, g_Blue2Title, g_PurpleTitle;

vector<CItem*> g_ListItems;					// danh sách các chi tiêu
WCHAR* g_DataFile = L"DATA.txt";			// tên file dữ liệu
CPie g_Pie[7];								// 7 góc trên biểu đồ
unsigned long long g_TypeCost[8] = { 0 };	// 7 loại chi tiêu và 1 cái tổng chi tiêu
float g_PercentPerType[7];					// tỷ lệ phần trăm mỗi loại chi tiêu

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


//_________________________________ FUNCION _________________________________

// Khởi tạo giao diện List View (cột, kích thước,...)
void InitListViewInterface()
{
	LVCOLUMN lvColumn[4];

	lvColumn[0].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[0].fmt = LVCFMT_LEFT;
	lvColumn[0].cx = 40;
	lvColumn[0].pszText = _T("STT");
	ListView_InsertColumn(g_ListBox, 0, &lvColumn[0]);

	lvColumn[1].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[1].fmt = LVCFMT_LEFT;
	lvColumn[1].cx = 100;
	lvColumn[1].pszText = _T("Loại");
	ListView_InsertColumn(g_ListBox, 1, &lvColumn[1]);

	lvColumn[2].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[2].fmt = LVCFMT_LEFT;
	lvColumn[2].cx = 220;
	lvColumn[2].pszText = _T("Nội dung");
	ListView_InsertColumn(g_ListBox, 2, &lvColumn[2]);

	lvColumn[3].mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn[3].fmt = LVCFMT_RIGHT;
	lvColumn[3].cx = 110;
	lvColumn[3].pszText = _T("Chi phí (vnđ)");
	ListView_InsertColumn(g_ListBox, 3, &lvColumn[3]);
}

// Thêm 1 item vào List View
void AddItemToListView(WCHAR* type, WCHAR* content, unsigned long long cost)
{
	// chuyển id thành chuỗi (để thêm vào list view)
	int id = g_ListItems.size() - 1;
	WCHAR* wID = new WCHAR[10];
	wsprintf(wID, L"%d", id + 1);

	// chuyển cost thành chuỗi (để thêm vào list view)
	wstring wCost = to_wstring(cost);

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = wID;
	lvItem.iItem = id;
	ListView_InsertItem(g_ListBox, &lvItem);
	ListView_SetItemText(g_ListBox, id, 1, type);
	ListView_SetItemText(g_ListBox, id, 2, content);
	ListView_SetItemText(g_ListBox, id, 3, (WCHAR*)wCost.c_str());
}

// Lưu dữ liệu lên file
void SaveDataToFile()
{
	wofstream fout;
	wstring a = L"ahha";

	fout.open(g_DataFile);
	fout.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));
	for (int i = 0; i < g_ListItems.size(); i++) {
		fout << g_ListItems[i]->GetType() << L"\n";
		fout << g_ListItems[i]->GetContent() << L"\n";
		fout << g_ListItems[i]->GetCost() << L"\n\n";
	}
	fout.close();
}

// Đổ dữ liệu từ file
void LoadDataFromFile()
{
	wifstream fin;
	wstring wLineType, wLineContent,wLineCost;
	unsigned long long cost;
	int id = 0;

	// mở file data để đọc
	fin.open(g_DataFile);
	fin.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));
	
	// đọc dữ liệu
	while (!fin.eof())
	{
		// đọc loại
		getline(fin, wLineType);
		// đọc content
		getline(fin, wLineContent);
		// đọc cost
		getline(fin, wLineCost);
		cost = _wtoll(wLineCost.c_str());

		// tới cuối file, thoát.
		if (wLineType == L"" || wLineContent == L"" || wLineCost == L"")
			break;

		// Cộng dồn tiền theo từng loại chi tiêu
		if (wLineType == L"Ăn uống")
			g_TypeCost[0] += cost;
		else if (wLineType == L"Di chuyển")
			g_TypeCost[1] += cost;
		else if (wLineType == L"Nhà cửa")
			g_TypeCost[2] += cost;
		else if (wLineType == L"Xe cộ")
			g_TypeCost[3] += cost;
		else if (wLineType == L"Nhu yếu phẩm")
			g_TypeCost[4] += cost;
		else if (wLineType == L"Dịch vụ")
			g_TypeCost[5] += cost;
		else
			g_TypeCost[6] += cost;
		g_TypeCost[7] += cost;

		// tạo mới 1 item
		CItem *item = new CItem(wLineType, wLineContent, cost);
		// thêm item vào danh sách
		g_ListItems.push_back(item);
		AddItemToListView((WCHAR*)wLineType.c_str(), (WCHAR*)wLineContent.c_str(), cost);

		// bỏ 1 dòng trống
		fin.ignore();
	}

	// nếu file có dữ liệu thì sẽ edit lại chú thích
	if (!g_ListItems.empty())
	{
		UpdateNoteForChart();
	}
	
	fin.close();
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
		else if (str[i] == ' ' && str[i + 1] == ' ')		// kí tự thứ i là ' ' thì xóa
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

// Kiểm tra loại chi tiêu người dùng nhập có thuộc 6 loại mặc định không
// Trả về:
//		+True: nếu thuộc 6 loại mặc định có sẵn (ăn uống, di chuyển, nhà cửa, xe cộ, nhu yếu phẩm, dịch vụ).
//		+False: nếu không thuộc 6 loại trên.
bool IsInDefaultType(wstring str)
{
	// kiểm tra có thuộc 6 loại mặc định không
	if (str == L"Ăn uống" || str == L"Di chuyển" || str == L"Nhà cửa" || str == L"Xe cộ"
		|| str == L"Nhu yếu phẩm" || str == L"Dịch vụ")
		return true;
	return false;
}

// Rút ngắn chuỗi số thực
void OptimizeFloatString(wstring &str)
{
	int len = str.length();
	for (int i = len-1; i >= 0; i--)
	{
		if (str[i] != L'0')
			break;
		else
		{
			str.erase(i, 1);
		}
	}
	// nếu số ko có phần thập phân thì xóa luôn kí tự '.'
	len = str.length();
	if (str[len - 1] == L'.')
		str.erase(len - 1, 1);
	// nếu chuỗi vẫn ở dạng xấp xỉ vô hạn
	len = str.length();
	if (len > 5)
		str.erase(5, len - 5);
}

// Cập nhật lại chú thích cho biểu đồ
void UpdateNoteForChart()
{
	wstring temp, text;
	float percent;
	if (g_TypeCost[7] > 0)
	{
		percent = (g_TypeCost[0] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Ăn uống (" + temp + L"%)";
		SetWindowText(g_RedTitle, text.c_str());

		percent = (g_TypeCost[1] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Di chuyển (" + temp + L"%)";
		SetWindowText(g_OrangeTitle, text.c_str());

		percent = (g_TypeCost[2] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Nhà cửa (" + temp + L"%)";
		SetWindowText(g_YellowTitle, text.c_str());

		percent = (g_TypeCost[3] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Xe cộ (" + temp + L"%)";
		SetWindowText(g_GreenTitle, text.c_str());

		percent = (g_TypeCost[4] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Nhu yếu phẩm (" + temp + L"%)";
		SetWindowText(g_Blue1Title, text.c_str());

		percent = (g_TypeCost[5] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Dịch vụ (" + temp + L"%)";
		SetWindowText(g_Blue2Title, text.c_str());

		percent = (g_TypeCost[6] / (g_TypeCost[7] * 1.0)) * 100;
		temp = to_wstring(roundf(percent * 100) / 100);
		OptimizeFloatString(temp);
		text = L"Khác (" + temp + L"%)";
		SetWindowText(g_PurpleTitle, text.c_str());
	}

	temp = to_wstring(g_TypeCost[7]);
	temp = AddDotToMoneyString(temp);
	text = L"Tổng chi tiêu: " + temp + L" (vnđ)";
	SetWindowText(g_SumBox, text.c_str());
}

// Thêm dấu chấm vào chuỗi tiền (dễ nhìn)
wstring AddDotToMoneyString(wstring str)
{
	wstring res = L"";
	int len = str.length();
	int countToAdd = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		res += str[i];
		countToAdd++;
		if (countToAdd == 3)
		{
			res += L".";
			countToAdd = 0;
		}
	}
	reverse(res.begin(), res.end());
	return res;
}


// ---------------------------- Các hàm window message ----------------------------
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(18, lf.lfWidth, lf.lfEscapement, lf.lfOrientation,
		lf.lfWeight, lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// lấy kích thước cửa sổ chính
	RECT rect;
	GetClientRect(hWnd, &rect);

	//---------group thêm dữ liệu (các ô điền thông tin + nút thêm)
	HWND AddGroupBox = CreateWindowW(L"BUTTON", L"Thêm 1 loại chi tiêu", BS_GROUPBOX |
		WS_CHILD | WS_VISIBLE, rect.right*0.46, 400, rect.right*0.53, 150, hWnd, NULL, hInst, NULL);

	HWND TypeLabel = CreateWindowW(L"STATIC", L"Loại chi tiêu", WS_CHILD | WS_VISIBLE,
		12, 45, 100, 20, AddGroupBox, NULL, hInst, NULL);

	g_TypeComboBox = CreateWindowW(L"COMBOBOX", L"", CBS_DROPDOWN | CBS_HASSTRINGS |
		WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 10, 70, 130, 100, AddGroupBox, NULL, hInst, NULL);

	HWND ContentLabel = CreateWindowW(L"STATIC", L"Nội dung", WS_CHILD | WS_VISIBLE,
		152, 45, 80, 20, AddGroupBox, NULL, hInst, NULL);

	g_ContentBox = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		150, 71, 200, 22, AddGroupBox, NULL, hInst, NULL);

	HWND CostLabel = CreateWindowW(L"STATIC", L"Số tiền", WS_CHILD | WS_VISIBLE,
		362, 45, 70, 20, AddGroupBox, NULL, hInst, NULL);

	g_CostBox = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER | ES_RIGHT | WS_BORDER,
		360, 71, 150, 22, AddGroupBox, NULL, hInst, NULL);

	g_AddButton = CreateWindowW(L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | BS_CENTER | BS_PUSHBUTTON,
		rect.right*0.67, rect.bottom - 55, 100, 30, hWnd, (HMENU)IDC_BUTTONADDITEM, hInst, NULL);

	//---------group xem dữ liệu (bảng list view show dữ liệu)
	HWND ListGroupBox = CreateWindowW(L"BUTTON", L"Danh sách các chi tiêu", BS_GROUPBOX |
		WS_CHILD | WS_VISIBLE, rect.right*0.46, 15, rect.right*0.53, 380, hWnd, NULL, hInst, NULL);

	g_ListBox = CreateWindowW(WC_LISTVIEW, _T("List View"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | WS_BORDER | LVS_REPORT,
		10, 20, 500, 350, ListGroupBox, NULL, hInst, NULL);

	//---------group xem biểu đồ (biểu đồ + chú thích + tổng chi tiêu)
	HWND ChartGroupBox = CreateWindowW(L"BUTTON", L"Biểu đồ thống kê", BS_GROUPBOX |
		WS_CHILD | WS_VISIBLE, 5, 15, rect.right*0.44, rect.bottom - 25, hWnd, NULL, hInst, NULL);

	g_RedTitle = CreateWindowW(L"STATIC", L"Ăn uống (0%)", WS_CHILD | WS_VISIBLE,
		75, 285, 160, 20, hWnd, NULL, hInst, NULL);

	g_OrangeTitle = CreateWindowW(L"STATIC", L"Di chuyển (0%)", WS_CHILD | WS_VISIBLE,
		75, 325, 160, 20, hWnd, NULL, hInst, NULL);

	g_YellowTitle = CreateWindowW(L"STATIC", L"Nhà cửa (0%)", WS_CHILD | WS_VISIBLE,
		75, 365, 160, 20, hWnd, NULL, hInst, NULL);

	g_Blue1Title = CreateWindowW(L"STATIC", L"Nhu yếu phẩm (0%)", WS_CHILD | WS_VISIBLE,
		75, 405, 200, 20, hWnd, NULL, hInst, NULL);

	g_GreenTitle = CreateWindowW(L"STATIC", L"Xe cộ (0%)", WS_CHILD | WS_VISIBLE,
		300, 285, 130, 20, hWnd, NULL, hInst, NULL);

	g_Blue2Title = CreateWindowW(L"STATIC", L"Dịch vụ (0%)", WS_CHILD | WS_VISIBLE,
		300, 325, 130, 20, hWnd, NULL, hInst, NULL);

	g_PurpleTitle = CreateWindowW(L"STATIC", L"Khác (0%)", WS_CHILD | WS_VISIBLE,
		300, 365, 130, 20, hWnd, NULL, hInst, NULL);

	g_SumBox = CreateWindowW(L"STATIC", L"Tổng chi tiêu: 0 vnđ", WS_CHILD | WS_VISIBLE,
		100, 480, 320, 20, hWnd, NULL, hInst, NULL);

	// Thêm lựa chọn cho danh sách của combo box
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Ăn uống");
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Di chuyển");
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Nhà cửa");
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Xe cộ");
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Nhu yếu phẩm");
	SendMessage(g_TypeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Dịch vụ");
	SendMessage(g_TypeComboBox, CB_SETCURSEL, 0, 0);

	// Set font
	SendMessage(TypeLabel, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_TypeComboBox, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(ContentLabel, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_ContentBox, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(CostLabel, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_CostBox, WM_SETFONT, WPARAM(hFont), TRUE);

	SendMessage(g_RedTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_OrangeTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_YellowTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_GreenTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_Blue1Title, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_Blue2Title, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_PurpleTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(g_SumBox, WM_SETFONT, WPARAM(hFont), TRUE);

	// Set tiền nhập tối đa 18 chữ số
	SendMessage(g_CostBox, EM_SETLIMITTEXT, 18, NULL);

	InitListViewInterface();
	LoadDataFromFile();
	GdiplusStartup(&token, &gdiplusStartupInput, NULL);
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BUTTONADDITEM:
	{
		wstring type, content;
		unsigned long long cost = 0;

		// lấy chiều dài các chuỗi trong 3 ô nhập
		int len1 = GetWindowTextLength(g_TypeComboBox);
		int len2 = GetWindowTextLength(g_ContentBox);
		int len3 = GetWindowTextLength(g_CostBox);

		// báo lỗi khi 1 trong 3 ô nhập bỏ trống
		if (len1 == 0 || len2 == 0 || len3 == 0) {
			MessageBox(hWnd, L"Bạn nhập thiếu thông tin cho dữ liệu, hãy thử lại!", L"Cảnh báo", MB_OK);
			break;
		}

		WCHAR *bufferType = new WCHAR[len1 + 1];
		WCHAR *bufferContent = new WCHAR[len2 + 1];
		WCHAR *bufferCost = new WCHAR[len3 + 1];

		// lấy chuỗi trong 3 ô nhập
		GetWindowText(g_TypeComboBox, bufferType, len1 + 1);
		GetWindowText(g_ContentBox, bufferContent, len2 + 1);
		GetWindowText(g_CostBox, bufferCost, len3 + 1);

		// ép kiểu về wstring
		type = wstring(bufferType);
		content = wstring(bufferContent);
		cost = _wtoll(bufferCost);

		// chuẩn hóa chuỗi do người dùng nhập
		StandardizeString(type);
		StandardizeString(content);

		// kiểm tra loại chi tiêu người dùng chọn, nếu không thuộc
		// 6 loại có sẵn sẽ đưa vào mục loại "Khác" (để đơn giản khi vẽ biểu đồ)
		if (!IsInDefaultType(type))
			type = L"Khác";

		// Cộng dồn tiền theo từng loại chi tiêu
		if (type == L"Ăn uống")
			g_TypeCost[0] += cost;
		else if (type == L"Di chuyển")
			g_TypeCost[1] += cost;
		else if (type == L"Nhà cửa")
			g_TypeCost[2] += cost;
		else if (type == L"Xe cộ")
			g_TypeCost[3] += cost;
		else if (type == L"Nhu yếu phẩm")
			g_TypeCost[4] += cost;
		else if (type == L"Dịch vụ")
			g_TypeCost[5] += cost;
		else
			g_TypeCost[6] += cost;
		g_TypeCost[7] += cost;

		// tạo mới 1 item
		CItem *item = new CItem(type, content, cost);

		// thêm item vào danh sách
		g_ListItems.push_back(item);
		AddItemToListView((WCHAR*)type.c_str(), (WCHAR*)content.c_str(), cost);

		// Vẽ lại biểu đồ
		InvalidateRect(hWnd, NULL, TRUE);

		// Cập nhật lại chú thích biểu đồ
		UpdateNoteForChart();

		// thông báo thêm thành công
		MessageBox(hWnd, L"Thêm 1 chi tiêu thành công!", L"Thông báo", MB_OK);

		// clear dữ liệu các ô nhập
		SendMessage(g_TypeComboBox, CB_SETCURSEL, 0, 0);
		SetWindowText(g_ContentBox, L"");
		SetWindowText(g_CostBox, L"");
	}
	break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	REAL start[7] = { 0.0 };
	REAL sweep[7] = { 0.0 };
	start[0] = -90;

	// TODO: Add any drawing code that uses hdc here...
	Graphics *graphics = new Graphics(hdc);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// Vẽ 7 pie tương ứng 7 loại chi tiêu
	if (g_TypeCost[7] == 0)
		graphics->DrawPie(&Pen(BLACK, 1), RectF(100, 40, 200, 200), -90, 0);
	else
	{
		for (int i = 0; i < 7; i++) {
			sweep[i] = (g_TypeCost[i] / (g_TypeCost[7] * 1.0)) * 360;
			if (i != 0)
				start[i] = start[i - 1] + sweep[i - 1];
			g_Pie[i] = CPie(i, start[i], sweep[i]);
			g_Pie[i].Draw(graphics);
		}
	}

	// Vẽ các 7 chú thích màu cho biểu đồ
	// cột 1 (4 màu)
	graphics->DrawPie(&Pen(BLACK, 1), RectF(5, 270, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(RED), RectF(5, 270, 60, 60), -45, 45);

	graphics->DrawPie(&Pen(BLACK, 1), RectF(5, 310, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(ORANGE), RectF(5, 310, 60, 60), -45, 45);

	graphics->DrawPie(&Pen(BLACK, 1), RectF(5, 350, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(YELLOW), RectF(5, 350, 60, 60), -45, 45);

	graphics->DrawPie(&Pen(BLACK, 1), RectF(5, 390, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(BLUE1), RectF(5, 390, 60, 60), -45, 45);

	// cột 2 (3 màu)
	graphics->DrawPie(&Pen(BLACK, 1), RectF(230, 270, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(GREEN), RectF(230, 270, 60, 60), -45, 45);

	graphics->DrawPie(&Pen(BLACK, 1), RectF(230, 310, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(BLUE2), RectF(230, 310, 60, 60), -45, 45);

	graphics->DrawPie(&Pen(BLACK, 1), RectF(230, 350, 60, 60), -45, 45);
	graphics->FillPie(&SolidBrush(PURPLE), RectF(230, 350, 60, 60), -45, 45);

	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd)
{
	GdiplusShutdown(token);
	SaveDataToFile();
	PostQuitMessage(0);
}
