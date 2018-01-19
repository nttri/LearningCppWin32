#pragma once

#include "CTag.h"

class CNote
{
	int ID;
	wstring title;
	wstring content;
	wstring tags;
	wstring dateTime;
public:
	CNote();
	CNote(int _id,wstring _title,wstring _content,wstring _tags,wstring _dateTime);
	void setID(int _id);
	void setTitle(wstring _title);
	void setContent(wstring _content);
	void setTags(wstring _tags);
	void setDateTime(wstring _dateTime);
	int getID();
	wstring getTitle();
	wstring getContent();
	wstring getTags();
	wstring getDateTime();
	~CNote();
};

