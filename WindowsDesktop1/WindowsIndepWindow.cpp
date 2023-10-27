//includes
/*
#include <windows.h> // win32 api
#include <stdlib.h> // standard library
#include <string.h> // standard string library
#include <tchar.h> // fancy strings
#include <corecrt_math.h> // math
#include "resource.h" // all resources used by the window

// the window class name
TCHAR szWindowClass[] = _T("MyWindowHere");

int clicked = 0;
TCHAR clicks[] = _T("000000");


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	HBITMAP g_Frac = NULL;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");
	TCHAR dothere[] = _T(".");

	switch (message) {
	case WM_CREATE:
		g_Frac = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
		if (g_Frac == NULL) MessageBox(hWnd, _T("Could not load IDB_BITMAP1!"), _T("Error"), NULL);
		break;
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

		for (int j = 0; j < 00; j = j + 13) {
			for (int i = 0; i < 00; i = i + 3) {
				TextOut(hdc,
					i, j,
					dothere, _tcslen(dothere));
			}
		}
		Rectangle(hdc, 40, 40, 110, 70);
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
		clicked++;
		for (int i = 0; i < 6; i++) {
			clicks[i] = wchar_t((clicked % int(pow(10, 6 - i)) / int(pow(10, 6 - i - 1))) + 48);
		}
		hdc = GetDC(hWnd);
		TextOut(hdc, 5, 15, greeting, _tcslen(greeting));
		TextOut(hdc, 50, 50, clicks, _tcslen(clicks));
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/****************************
* wWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
* hInstance: an additional identifier for win32 api calls
* hPrevInstance: alawys 0, was used in win16 
* lpCmdLine: Command line arguments as an Unicode string
* nCmdShow: flag for the window to be maximized, minimuzed, or shown normally
****************************/
/*
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wcex; // contains window class information
	wcex.cbSize = sizeof(WNDCLASSEX); // the size in bytes of the class. always this
	wcex.style = CS_HREDRAW | CS_VREDRAW; // can use any of the windows class styles
	wcex.lpfnWndProc = WndProc; // a pointer to the windows procedure (message loop)
	wcex.cbClsExtra = 0; // extra bytes to allocate following the wndclassex class
	wcex.cbWndExtra = 0; // extra bytes to allocate following the window instance
	wcex.hInstance = hInstance; // an additional identifier for win32 api calls 
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION); // icon
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // cursor over the window
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // background color
	wcex.lpszMenuName = NULL; // menu resource
	wcex.lpszClassName = szWindowClass; // window class name 
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // small icon

	// attempt to register class. if does not work error
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Application"), NULL);
		return 1;
	}

	// HWND is the handle to the window that is attempted to be created
	HWND hWnd = CreateWindowEx( // returns NULL if unsucessful
		WS_EX_OVERLAPPEDWINDOW, // an optional extended window style
		szWindowClass, // the class of the window being created
		_T("Windows Desktop Application"), // the Text to appear in the title bar
		WS_OVERLAPPEDWINDOW, // the kind of window to create?
		CW_USEDEFAULT, CW_USEDEFAULT, // initial x and y position of window (top left)
		525, 555, // width and height
		NULL, // the parent of this window
		NULL, // the menu bar of this window
		hInstance, // the first parameter of winMain (an additional identifer)
		NULL // "Pointer to a value to be passed to the window through the CREATESTRUCT structure"
	);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Desktop Application"), NULL);
		return 1;
	}
	//the window to show, how it is being showed (min, max, normal
	ShowWindow(hWnd, nCmdShow);
	// sends the WM_PAINT message to the window created
	UpdateWindow(hWnd);

	// the main message loop
	MSG msg; // a windows message
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
*/


