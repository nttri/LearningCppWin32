#include "stdafx.h"
#include "CTag.h"


CTag::CTag()
{
	this->name = L"";
	this->NoteIDs = { 0 };
}

CTag::CTag(wstring _name, vector<int> _noteIDs)
{
	this->name = _name;
	this->NoteIDs = _noteIDs;
}

wstring CTag::getName()
{
	return this->name;
}

void CTag::setName(wstring _name)
{
	this->name = _name;
}

vector<int> CTag::getNoteIDs()
{
	return this->NoteIDs;
}

void CTag::setNoteIDs(vector<int> _noteIDs)
{
	this->NoteIDs = _noteIDs;
}

void CTag::addNoteIdToTag(int _noteID)
{
	NoteIDs.push_back(_noteID);
}

void CTag::delNoteID(int _noteID)
{
	vector<int> temp;
	for (int i = 0; i < NoteIDs.size(); i++)
	{
		if (NoteIDs[i] != _noteID)
			temp.push_back(NoteIDs[i]);
	}
	NoteIDs = temp;
}

void CTag::increaseNoteIDAfterNoteID(int _noteID)
{
	bool isExist = false;
	for (int i = 0; i < NoteIDs.size(); i++)
	{
		if (NoteIDs[i] == _noteID)
			isExist = true;
	}
	if (isExist)
	{
		for (int i = 0; i < NoteIDs.size(); i++)
		{
			if (NoteIDs[i] > _noteID)
				NoteIDs[i]--;
		}
	}
}

int CTag::countNote()
{
	return NoteIDs.size();
}


CTag::~CTag()
{
}
