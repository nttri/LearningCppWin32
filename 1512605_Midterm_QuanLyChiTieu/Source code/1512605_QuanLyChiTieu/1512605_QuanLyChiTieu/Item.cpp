#include "stdafx.h"
#include "Item.h"


CItem::CItem()
{
}

CItem::CItem(wstring type, wstring content, unsigned long long cost)
{
	strType = type;
	strContent = content;
	lCost = cost;
}

wstring CItem::GetType()
{
	return strType;
}


wstring CItem::GetContent()
{
	return strContent;
}

unsigned long long CItem::GetCost()
{
	return lCost;
}

void CItem::SetType(wstring type)
{
	strType = type;
}

void CItem::SetContent(wstring content)
{
	strContent = content;
}

void CItem::SetCost(unsigned long long cost)
{
	lCost = cost;
}

CItem::~CItem()
{
}
