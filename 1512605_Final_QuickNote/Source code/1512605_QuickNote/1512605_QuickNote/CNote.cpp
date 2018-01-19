#include "stdafx.h"
#include "CNote.h"


CNote::CNote()
{
	this->ID = 0;
	this->title = L"";
	this->content = L"";
	this->tags = L"";
}

CNote::CNote(int _id, wstring _title, wstring _content, wstring _tags, wstring _dateTime)
{
	this->ID = _id;
	this->title = _title;
	this->content = _content;
	this->tags = _tags;
	this->dateTime = _dateTime;
}

void CNote::setID(int _id)
{
	this->ID = _id;
}

void CNote::setTitle(wstring _title)
{
	this->title = _title;
}

void CNote::setContent(wstring _content)
{
	this->content = _content;
}

void CNote::setTags(wstring _tags)
{
	this->tags = _tags;
}

void CNote::setDateTime(wstring _dateTime)
{
	this->dateTime = _dateTime;
}

int CNote::getID()
{
	return this->ID;
}

wstring CNote::getTitle()
{
	return this->title;
}

wstring CNote::getContent()
{
	return this->content;
}

wstring CNote::getTags()
{
	return this->tags;
}

wstring CNote::getDateTime()
{
	return this->dateTime;
}


CNote::~CNote()
{
}
