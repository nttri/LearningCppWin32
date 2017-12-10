// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
HWND g_hWnd;
HHOOK g_hook;
HINSTANCE g_hIns;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hIns = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL InitMouseHook(HWND hWnd)
{
	g_hWnd = hWnd;
	g_hook = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hIns, 0);
	if (g_hook != NULL)
	{
		return true;
	}
	return false;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			return -1;
		default:
			break;
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

BOOL UninstallMouseHook()
{
	if (g_hook != NULL)
	{
		UnhookWindowsHookEx(g_hook);
		return true;
	}
	return false;
}