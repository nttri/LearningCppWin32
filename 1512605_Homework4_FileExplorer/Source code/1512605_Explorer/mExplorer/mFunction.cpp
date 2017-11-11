#include "stdafx.h"
#include "mExplorer.h"


void Init()
{
	GetDisk();
	HTREEITEM thisPC = InitThisPC();
	InitTreeView(thisPC);
	InitListViewInterface();
}


void GetDisk()
{
	WCHAR allDiskString[200];
	WCHAR *perDiskString = new WCHAR[3];
	GetLogicalDriveStrings(200, allDiskString);
	int pos = 0;

	// nếu không có ổ đĩa sẽ báo lỗi
	if (wcslen(allDiskString) == 0)
	{
		MessageBox(hwnd, L"Không tìm được ổ đĩa nào. Kiểm tra lại!", L"LỖI", MB_ICONERROR | MB_OK);
		DestroyWindow(hwnd);
		return;
	}

	do
	{
		wcsncpy(perDiskString, allDiskString + pos, 2);
		perDiskString[2] = 0;
		wcscpy(Diskk[nDisk++], perDiskString);
		// nhảy qua 4 ký tự (vd: 'C',':','\\','\0')
		pos += 4;
	} while (allDiskString[pos] != 0);
}


HTREEITEM InitThisPC()
{
	TVINSERTSTRUCT thisPCStruct;
	thisPCStruct.hParent = TVI_ROOT;
	thisPCStruct.hInsertAfter = TVI_FIRST;
	thisPCStruct.item.iImage = 29;
	thisPCStruct.item.iSelectedImage = 29;
	thisPCStruct.item.cchTextMax = 255;
	thisPCStruct.item.lParam = (LPARAM)L"This PC";
	thisPCStruct.item.pszText = L"This PC";
	thisPCStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&thisPCStruct);
}


HTREEITEM AddItemToTreeView(WCHAR *path, HTREEITEM item, WCHAR * folderName)
{
	TVINSERTSTRUCT nodeStruct;
	nodeStruct.hParent = item;
	nodeStruct.hInsertAfter = item;
	// kiểm tra phân loại để chọn icon
	// ổ đĩa thì path chỉ có 2 kí tự (vd: "C:")
	if (path[2] == '\0')
	{
		nodeStruct.item.iImage = 13;
		nodeStruct.item.iSelectedImage = 13;
	}
	// còn lại
	else
	{
		nodeStruct.item.iImage = 31;
		nodeStruct.item.iSelectedImage = 31;
	}
	nodeStruct.item.cchTextMax = 255;
	nodeStruct.item.lParam = (LPARAM)path;
	nodeStruct.item.pszText = folderName;
	nodeStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&nodeStruct);
}


void InitTreeView(HTREEITEM thisPC)
{
	for (int i = 0; i < nDisk; i++)
		AddItemToTreeView(Diskk[i], thisPC, Diskk[i]);
}


void BrowseFolder(WCHAR* parent, HTREEITEM a)
{
	WIN32_FIND_DATA data;
	WCHAR folderPath[MAX_PATH];
	wsprintf(folderPath, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folderPath, &data);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(data.cFileName, L"..") && wcscmp(data.cFileName, L"."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(data.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, data.cFileName);
					AddItemToTreeView(child, a, data.cFileName);
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}


void InitListViewInterface()
{
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;

	lvColumn.cx = 180;
	lvColumn.pszText = _T("Name");
	ListView_InsertColumn(hList, 0, &lvColumn);

	lvColumn.cx = 120;
	lvColumn.pszText = _T("Type");
	ListView_InsertColumn(hList, 1, &lvColumn);

	lvColumn.cx = 80;
	lvColumn.pszText = _T("Size");
	ListView_InsertColumn(hList, 2, &lvColumn);

	lvColumn.cx = 130;
	lvColumn.pszText = _T("Date modified");
	ListView_InsertColumn(hList, 3, &lvColumn);
}


void InitListView()
{
	for (int i = 0; i < nDisk; i++)
		AddDiskToListView(Diskk[i], i);
}


void AddDiskToListView(LPWSTR _disk, int iRows)
{
	long long nSize = 0;
	SHGetDiskFreeSpaceEx(_disk, NULL, (PULARGE_INTEGER)&nSize, NULL);
	LVITEM diskInLV;
	diskInLV.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	diskInLV.iSubItem = 0;
	diskInLV.cchTextMax = 2;
	diskInLV.pszText = _disk;
	diskInLV.iImage = 13;
	diskInLV.iItem = iRows;
	diskInLV.lParam = (LPARAM)_disk;
	ListView_InsertItem(hList, &diskInLV);
	ListView_SetItemText(hList, iRows, 3, L"");
	ListView_SetItemText(hList, iRows, 1, L"Disk");
	ListView_SetItemText(hList, iRows, 2, Convert(nSize));
}


LPWSTR Convert(__int64 nSize)
{
	int nType = 0;
	while (nSize >= 1048576)
	{
		nSize /= 1024;
		++nType;
	}
	__int64 nRight;
	if (nSize >= 1024)
	{
		nRight = nSize % 1024;
		while (nRight > 99)
			nRight /= 10;
		nSize /= 1024;
		++nType;
	}
	else
		nRight = 0;
	TCHAR *buffer = new TCHAR[11];
	_itow_s(nSize, buffer, 11, RADIX);
	if (nRight != 0 && nType > KB)
	{
		wcscat(buffer, L".");
		TCHAR *right = new TCHAR[3];
		_itow_s(nRight, right, 3, RADIX);
		wcscat(buffer, right);
	}
	if (buffer[0] == L'0')
		return L"";
	switch (nType)
	{
	case 0://Bytes
		wcscat(buffer, L" bytes");
		break;
	case KB:
		wcscat(buffer, L" KB");
		break;
	case MB:
		wcscat(buffer, L" MB");
		break;
	case GB:
		wcscat(buffer, L" GB");
		break;
	case TB:
		wcscat(buffer, L" TB");
		break;
	}
	return buffer;
}

#define ENTIRE_STRING NULL

LPWSTR _GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len)
		return _T("File Folder");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}
	RegCloseKey(hKey);

	return pszPath;
}

time_t ConvertTime(const FILETIME& ft)
{
	ULARGE_INTEGER uli;
	uli.LowPart = ft.dwLowDateTime;
	uli.HighPart = ft.dwHighDateTime;
	return uli.QuadPart / 10000000ULL - 11644473600ULL;
}

void AddItemToListView(WCHAR* link, WIN32_FIND_DATA _data, int iRows)
{
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.pszText = _data.cFileName;
	if(_data.nFileSizeLow != 0)
		lvItem.iImage = 11;
	else
		lvItem.iImage = 31;
	lvItem.lParam = (LPARAM)link;
	lvItem.cchTextMax = 100;
	lvItem.iItem = iRows;
	ListView_InsertItem(hList, &lvItem);
	time_t Time = ConvertTime(_data.ftLastWriteTime);
	tm *ltm = localtime(&Time);
	WCHAR *wstr = new WCHAR[20];
	if (ltm->tm_hour == 12)
		wsprintf(wstr, L"%d/%d/%d %d:%d PM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min);
	else if (ltm->tm_hour < 12)
		wsprintf(wstr, L"%d/%d/%d %d:%d AM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min);
	else
		wsprintf(wstr, L"%d/%d/%d %d:%d PM", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, ltm->tm_hour - 12, ltm->tm_min);
	ListView_SetItemText(hList, iRows, 3, wstr);
	WCHAR* typeStr = _GetType(_data);
	if (typeStr == L"File Folder" && _data.nFileSizeLow != 0)
	{
		ListView_SetItemText(hList, iRows, 1, L"File");
		ListView_SetItemText(hList, iRows, 2, Convert(_data.nFileSizeLow));
	}
	else
	{
		ListView_SetItemText(hList, iRows, 1, _GetType(_data));
		ListView_SetItemText(hList, iRows, 2, Convert(_data.nFileSizeLow));
	}
}

void LoadCurSel(WCHAR *path)
{
	WIN32_FIND_DATA fd;

	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(hList);
		AddList(path);
	}
	else //Nếu là tập tin thì chạy nó
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}

void AddList(LPWSTR parent)
{
	WIN32_FIND_DATA fd;
	WCHAR folder[MAX_PATH];
	wsprintf(folder, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folder, &fd);
	int dem = 0;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (!(fd.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN))
			{
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L".."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(fd.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, fd.cFileName);
					AddItemToListView(child, fd, dem);
					dem++;
				}
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}