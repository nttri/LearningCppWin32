#pragma once

#include "1512605_QuickNote.h"

class CTag
{
	wstring name;
	vector<int> NoteIDs;
public:
	CTag();
	CTag(wstring _name, vector<int> _noteID);
	wstring getName();
	void setName(wstring _name);
	vector<int> getNoteIDs();
	void setNoteIDs(vector<int> _noteIDs);
	void addNoteIdToTag(int _noteID);
	void delNoteID(int _noteID);
	void increaseNoteIDAfterNoteID(int _noteID);
	int countNote();
	~CTag();
};

