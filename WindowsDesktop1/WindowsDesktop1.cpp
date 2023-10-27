#include "WindowsDesktop1.h"

TCHAR szWindowClass[] = _T("WindowsDesktop1");
TCHAR szWindowGraph[] = _T(windowGraph);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCE(IDC_WINDOWSDESKTOP1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

	WNDCLASSEX wcexG;
	wcexG.cbSize = sizeof(WNDCLASSEX);
	wcexG.style = CS_HREDRAW | CS_VREDRAW;
	wcexG.lpfnWndProc = WndGraphProc; //use the same message function
	wcexG.cbClsExtra = 0;
	wcexG.cbWndExtra = 0;
	wcexG.hInstance = hInstance; 
	wcexG.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcexG.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexG.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcexG.lpszMenuName = NULL;
	wcexG.lpszClassName = szWindowGraph;
	wcexG.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// attempt to register class.
	if (!RegisterClassEx(&wcex) || !RegisterClassEx(&wcexG)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Application"), NULL);
		return 1;
	}

	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, szWindowClass, _T("Windows Desktop Application"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		525, 555,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Desktop Application"), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}