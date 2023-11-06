// this is also a file

#include "WindowsDesktop1.h"

int clicked = 0;
char saveFile[] = "DesktopSave.txt";
TCHAR mmoti[] = _T("000x000");
HBITMAP g_Frac = NULL;
HWND hEdit;
HWND hEditBox;
HWND hProcess;
HWND hSave;
HWND hLoad;
HWND hToggle;
HWND hGraph;
HWND Graph;

HWND test;

bool hToggleDisplay = true;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");
	TCHAR dothere[] = _T(".");

	switch (message) {
	case WM_CREATE:
	{
		g_Frac = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
		if (g_Frac == NULL) MessageBox(hWnd, _T("Could not load IDB_BITMAP1!"), _T("Error"), NULL);

		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, //WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
			100, 200, 80, 20, hWnd, (HMENU)IDM_FILE_EDIT, GetModuleHandle(NULL), NULL);
		hEditBox = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
			WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, //WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
			100, 290, 80, 20, hWnd, (HMENU)IDB_EDIT_BOX, GetModuleHandle(NULL), NULL);
		hProcess = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Process"),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			200, 200, 80, 20, hWnd, (HMENU)BTN_PROCESS, GetModuleHandle(NULL), NULL);
		hSave = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Save"),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			200, 230, 80, 20, hWnd, (HMENU)BTN_SAVE, GetModuleHandle(NULL), NULL);
		hLoad = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Load"),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			200, 260, 80, 20, hWnd, (HMENU)BTN_LOAD, GetModuleHandle(NULL), NULL);
		hGraph = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Graph"),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			200, 290, 80, 20, hWnd, (HMENU)BTN_GRAPH, GetModuleHandle(NULL), NULL);
		hToggle = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Edit Toggle"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			120, 055, 80, 20, hWnd, (HMENU)BTN_CLOSEEDIT, GetModuleHandle(NULL), NULL);

		clicked = loadClick(saveFile);
		break;
	}
	case WM_PAINT:
	{
		BITMAP bm;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);

		HDC hdcMem = CreateCompatibleDC(hdc);
		HGDIOBJ hbmOld = SelectObject(hdcMem, g_Frac);

		GetObject(g_Frac, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

		RECT clickBack = { 40, 40, 110, 70 };
		DrawEdge(hdc, &clickBack, EDGE_BUMP, BF_RECT | BF_MIDDLE);
		//Rectangle(hdc, 40, 40, 110, 70);
		TextOut(hdc, 5, 15, greeting, _tcslen(greeting));
		updateClick(clicked, hWnd);
		EndPaint(hWnd, &ps);

		break;
	}
	case WM_GETTEXT:
	{
		TCHAR equation[] = _T("00000000000000000000000");
		SendMessage(hEditBox, WM_GETTEXT, _tcslen(equation), (LPARAM)equation);
		TCHAR* reqEquation = (TCHAR*)lParam;

		//MessageBox(hWnd, equation, _T("Windows Desktop Application"), NULL);

		for (int i = 0; i < min(_tcslen(equation), _tcslen(reqEquation)); i++) {
			reqEquation[i] = equation[i];
		}

		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BTN_PROCESS:
		{
			TCHAR clicksWanted[] = _T("0000000");
			SendMessage(hEdit, WM_GETTEXT, _tcslen(clicksWanted), (LPARAM)clicksWanted);
			int clicksProc = charToInt(clicksWanted, _tcslen(clicksWanted));
			if (clicksProc < 0) {
				MessageBox(hWnd, _T("Enter a number!"), _T("Windows Desktop Application"), NULL);
			}
			clicked += clicksProc;
			updateClick(clicked, hWnd);


			//time_t now = std::time(0);
			//std::wstring reslTime = timeToDate(&now);

			//MessageBox(hWnd, reslTime.c_str(), _T("Windows Desktop Application"), NULL);
			break;
		}
		case BTN_CLOSEEDIT:
			hToggleDisplay = !hToggleDisplay;
			if (!hToggleDisplay) {
				ShowWindow(hEdit, SW_HIDE);
				ShowWindow(hProcess, SW_HIDE);
				ShowWindow(hSave, SW_HIDE);
				ShowWindow(hLoad, SW_HIDE);
				ShowWindow(hGraph, SW_HIDE);
				ShowWindow(hEditBox, SW_HIDE);
			break;
			}
			else {
				ShowWindow(hEdit, SW_SHOW);
				ShowWindow(hProcess, SW_SHOW);
				ShowWindow(hSave, SW_SHOW);
				ShowWindow(hLoad, SW_SHOW);
				ShowWindow(hGraph, SW_SHOW);
				ShowWindow(hEditBox, SW_SHOW);
				break;
			}
		case BTN_SAVE:
		{
			saveClick(saveFile, clicked);
			break;
		}
		case BTN_LOAD:
		{
			clicked = loadClick(saveFile);
			updateClick(clicked, hWnd);
			break;
		}
		case BTN_GRAPH:
		{
			int grwidth = 700;
			int grheigh = 700;
			Graph = CreateWindowEx(
				WS_EX_STATICEDGE, _T(windowGraph), _T("Windows Desktop Application"),
				WS_OVERLAPPEDWINDOW, //WS_SIZEBOX, //WS_POPUP
				GetSystemMetrics(SM_CXSCREEN) / 2 - grwidth / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - grheigh / 2,
				grwidth, grheigh,
				hWnd, NULL, GetModuleHandle(NULL), NULL);
		break;
		}
		}
		break;
	case WM_LBUTTONDOWN:
		clicked++;
		updateClick(clicked, hWnd);
		break;
	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hWnd);
		int x_coo = LOWORD(lParam);
		int y_coo = HIWORD(lParam);
		intToChar(x_coo, mmoti, 3);
		intToChar(y_coo, mmoti + 4, 3);
		TextOut(hdc, 100, 100, mmoti, _tcslen(mmoti));
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}