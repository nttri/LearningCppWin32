#pragma once

#include "resource.h"
#include "Item.h"
#include "Pie.h"
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <CommCtrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100

//_________________________________ Variable _________________________________
extern HWND g_TypeComboBox, g_ContentBox, g_CostBox, g_AddButton, g_ListBox, g_SumBox,
g_RedTitle, g_OrangeTitle, g_YellowTitle, g_GreenTitle, g_Blue1Title, g_Blue2Title, g_PurpleTitle;
extern vector<CItem*> g_ListItems;			// danh sách chứa các chi tiêu
extern WCHAR* g_DataFile;					// tên file dữ liệu
extern unsigned long long g_TypeCost[8];	// 7 loại chi tiêu và 1 cái tổng chi tiêu
extern float g_PercentPerType[7];			// tỷ lệ phần trăm mỗi loại chi tiêu

//_________________________________ Function _________________________________

void InitListViewInterface();													// Khởi tạo giao diện List View (cột, kích thước,...)
void AddItemToListView(WCHAR* type, WCHAR* content, unsigned long long cost);	// Thêm 1 item vào List View
void SaveDataToFile();															// Lưu dữ liệu lên file
void LoadDataFromFile();														// Đổ dữ liệu từ file
void StandardizeString(wstring &str);											// Chuẩn hóa chuỗi (xóa khoảng trắng thừa)
bool IsInDefaultType(wstring str);												// Kiểm tra loại chi tiêu người dùng nhập có thuộc 6 loại mặc định không
void OptimizeFloatString(wstring &str);											// Rút ngắn chuỗi số thực
void UpdateNoteForChart();														// Cập nhật lại chú thích cho biểu đồ
wstring AddDotToMoneyString(wstring str);										// Thêm dấu chấm vào chuỗi tiền (dễ nhìn)

// Window Message
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);

