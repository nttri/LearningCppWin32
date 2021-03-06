#pragma once

#include "resource.h"
#include <string>
#include <windowsx.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <codecvt>
#include <CommCtrl.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace Gdiplus;
using namespace std;

#define BLACK Color(0,0,0)
#define WHITE Color(255,255,255)
#define GREENL Color(17, 153, 142)
#define GREENR Color(56, 239, 125)
#define TRANS Color(60,0,0,0)
#define RED Color(255,0,0)
#define BLUEGREY Color(38, 50, 56)
#define RANK1CLR Color(241, 196, 15)
#define RANK2CLR Color(41, 182, 246)
#define RANK3CLR Color(255, 112, 67)
#define RANK4CLR Color(216, 136, 234)
#define RANK5CLR Color(232, 118, 156)

LRESULT CALLBACK CatchKeyDown(int message, WPARAM wParam, LPARAM lParam);

// WM cho cửa sổ 1
void OnDrawItem1(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);
void OnDestroy1(HWND hwnd);
BOOL OnCreate1(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand1(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint1(HWND hwnd);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);

// WM cho cửa sổ 2
void OnDrawItem2(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);
void OnDestroy2(HWND hwnd);
BOOL OnCreate2(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand2(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint2(HWND hwnd);

// WM cho cửa sổ 3
void OnDestroy3(HWND hwnd);
BOOL OnCreate3(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnPaint3(HWND hwnd);

// hàm hỗ trợ
void init();												// khởi tạo những thứ cần thiết
HTREEITEM initRootInTV();									// tạo root cho tree view tags
void AddItemToTreeView(HTREEITEM root, WCHAR *tagName);		// thêm 1 tag vào treeview
void InitListViewInterface();								// Khởi tạo giao diện List View (cột, kích thước,...)
void AddItemToListView(int i,int idNote, WCHAR* title, WCHAR* content, WCHAR* tag, WCHAR* date);	// Thêm 1 item vào List View
void StandardizeString(wstring &str);						// Chuẩn hóa chuỗi (xóa khoảng trắng thừa)
void delRedunComma(wstring &str);							// Xóa dấu ',' thừa trong 1 chuỗi
vector<wstring> getTagList(wstring str);					// lấy danh sách các tag từ 1 chuỗi (các tag phân cách bằng dấu ',', 
															// trường hợp chuỗi có chuỗi con có nhiều kí tự ',' liên tiếp nhau 
															// thì chuỗi con đó sẽ không tính là 1 tag)
void SaveTagToFile();										// Lưu dữ liệu tag lên file
void LoadTagFromFile();										// Đổ dữ liệu từ file tag
void SaveNoteToFile();										// Lưu dữ liệu note lên file
void LoadNoteFromFile();									// Đổ dữ liệu từ file note
BOOL AddNotificationIcon(HWND hwnd, HINSTANCE hInstance);	// thêm chương trình vào notification area
wstring getCurrentDateTime();								// lấy ngày giờ hiện tại của hệ thống


