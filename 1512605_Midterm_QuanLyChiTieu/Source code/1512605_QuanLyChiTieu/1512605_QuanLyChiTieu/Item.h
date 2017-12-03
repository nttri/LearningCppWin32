#pragma once
#include <string>
using namespace std;

class CItem
{
private:
	wstring strType;
	wstring strContent;
	unsigned long long lCost;
public:
	CItem();
	CItem(wstring type, wstring content, unsigned long long cost);
	wstring GetType();
	wstring GetContent();
	unsigned long long GetCost();
	void SetType(wstring type);
	void SetContent(wstring content);
	void SetCost(unsigned long long cost);
	~CItem();
};

