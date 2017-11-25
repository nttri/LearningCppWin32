// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <UIRibbon.h>

#include "1512605_PaintGDIplus.h"
#include "CommandHandler.h"
#include "RibbonIDs.h"
#include "RibbonFramework.h"

#include <uiribbonpropertyhelpers.h>
#include "Propvarutil.h"
#include "stdlib.h"
#pragma comment (lib,"Propsys.lib")

// Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(IUICommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}

	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	REFPROPERTYKEY key,
	const PROPVARIANT* ppropvarCurrentValue,
	PROPVARIANT* ppropvarNewValue)
{
	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(ppropvarCurrentValue);
	UNREFERENCED_PARAMETER(ppropvarNewValue);
	return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(nCmdID);

	switch (nCmdID)
	{
	case ID_BTN_LINE:
	case ID_BTN_RECT:
	case ID_BTN_ELLIPSE:
		checkToggle(ppropvarValue, nCmdID);
		break;
	}

	SendMessage(GetForegroundWindow(), WM_COMMAND, nCmdID, 0);

	return S_OK;
}

void checkToggle(const PROPVARIANT* ppropvarValue, int idCommand)
{
	PROPVARIANT propvar;
	PROPVARIANT var;
	PROPVARIANT varNew;
	int cmdGroupShapeColor[3] = {ID_BTN_LINE , ID_BTN_RECT, ID_BTN_ELLIPSE};

	HRESULT hr;
	BOOL boolValue;

	int selected = ppropvarValue->uintVal;
	hr = g_pFramework->GetUICommandProperty(idCommand, UI_PKEY_BooleanValue, &var);
	hr = PropVariantToBoolean(var, &boolValue);


	PropVariantInit(&propvar);
	if (boolValue != 1)
	{
		boolValue = !boolValue;
		hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, boolValue, &varNew);
		hr = g_pFramework->SetUICommandProperty(idCommand, UI_PKEY_BooleanValue, varNew);
		return;

	}
	boolValue = !boolValue;
	hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, boolValue, &varNew);

	for (int i = 0; i < 3; i++)
	{
		if (cmdGroupShapeColor[i] != idCommand)
			hr = g_pFramework->SetUICommandProperty(cmdGroupShapeColor[i], UI_PKEY_BooleanValue, varNew);
	}
}
