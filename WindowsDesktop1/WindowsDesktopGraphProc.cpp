// this is also a file




#include "WindowsDesktop1.h"


RECT windowBounds;
struct GraphProp gr;


HWND hWndPare;

float custdxy(float x, float y) {
	return x + y;
}
float custy(float y) {
	return y + 1;
}
float custx(float x) {
	return x;
}

struct Func* f1 = NULL;

bool hold;
int holdx;
int holdy;

float uppx;
float lowx;
float uppy;
float lowy;

TCHAR cursorxy[] = _T("(00000, 00000)");
int chh;

LRESULT CALLBACK WndGraphProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	HDC hdcbuf;
	HBITMAP bitbuf;

	switch (message) {
	case WM_CREATE:
	{
		hold = false;
		holdx = 0;
		holdy = 0;

		uppx = 0;
		lowx = 0;
		uppy = 0;
		lowy = 0;

		chh = 0;

		gr = {
				30, 30,			// x buff, y buff
				0, 6.3, 0.01,	//t bounds and res(percent)
				1,				// isPara 0 yes, -1 simply y, other simply x
				-5, 5,			// x bounds
				-5, 5,			// y bounds
				&windowBounds
		};

		hWndPare = GetWindow(hWnd, GW_OWNER);
		ShowWindow(hWndPare, SW_HIDE);

		TCHAR equationWanted[] = _T("000000000000000000000000000000000000000");
		SendMessage(hWndPare, WM_GETTEXT, _tcslen(equationWanted), (LPARAM)equationWanted);
		//int eqTEst = charToInt(equationWanted, _tcslen(equationWanted));
		//MessageBox(hWnd, equationWanted, _T("Windows Desktop Application"), NULL);

		char equation[40] = { 0 };
		for (int i = 0; i < 40; i++) {
			equation[i] = equationWanted[i];
		}

		f1 = compileFunc(40, equation, 0, 0);

		TCHAR test22[] = _T("                             ");
		intToChar(evalFunct(f1, 0), test22, _tcslen(test22));
		//MessageBox(hWnd, test22, _T("Windows Desktop Application"), NULL);
		
		UpdateWindow(hWnd);
		ShowWindow(hWnd, SW_SHOW);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		updateWindowBounds(hWnd, gr);
		hdc = BeginPaint(hWnd, &ps);
		hdcbuf = CreateCompatibleDC(hdc);
		bitbuf = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
		SelectObject(hdcbuf, bitbuf);



		if (ps.rcPaint.top < gr.bounding->bottom) {
			HPEN pen = CreatePen(PS_SOLID, 3, chroma(chh));
			HGDIOBJ oldpen = SelectObject(hdcbuf, pen);

			
			if (f1 != NULL) {
				updateNMIGraph(hdcbuf, f1, gr);
			}
			
			pen = CreatePen(PS_SOLID, 2, chroma(chh + 1));
			SelectObject(hdcbuf, pen);
			
			//updateGraph(hdcbuf, &custx, &custy, gr);
			
			pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
			SelectObject(hdcbuf, pen);
		    
			updateAxis(hdcbuf, gr);
			

			TCHAR lower[] = _T("(00000, 00000)");
			floatToChar(gr.lowerx, lower + 1, 5);
			floatToChar(gr.lowery, lower + 8, 5);
			TextOut(hdcbuf, gr.bounding->left, gr.bounding->bottom + 1, lower, _tcslen(lower));
			floatToChar(gr.upperx, lower + 1, 5);
			floatToChar(gr.uppery, lower + 8, 5);
			TextOut(hdcbuf, gr.bounding->right - 100, gr.bounding->top - 20, lower, _tcslen(lower));
			
			SelectObject(hdcbuf, oldpen);
			DeleteObject(pen);
			DeleteObject(oldpen);
		} else {
			TextOut(hdcbuf, 0, 0, cursorxy, _tcslen(cursorxy));
		}




		BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hdcbuf, 0, 0, SRCCOPY);
		DeleteObject(bitbuf);
		DeleteDC(hdcbuf);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		hold = true;
		holdx = LOWORD(lParam);
		holdy = HIWORD(lParam);

		uppx = gr.upperx;
		lowx = gr.lowerx;
		uppy = gr.uppery;
		lowy = gr.lowery;

		break;
	}
	case WM_MOUSEMOVE:
	{
		if (hold) {

			float difx = LOWORD(lParam) - holdx;
			float dify = HIWORD(lParam) - holdy;

			difx = difx * (gr.upperx - gr.lowerx) / (float) (gr.bounding->right - gr.bounding->left);
			dify = dify * (gr.uppery - gr.lowery) / (float) (gr.bounding->bottom - gr.bounding->top);

			gr.upperx = uppx - difx;
			gr.lowerx = lowx - difx;
			gr.uppery = uppy + dify;
			gr.lowery = lowy + dify;
			InvalidateRect(hWnd, NULL, false);
		}
		else {
			floatToChar(toAxis(LOWORD(lParam), gr, true), cursorxy + 1, 5);
			floatToChar(toAxis(HIWORD(lParam), gr, false), cursorxy + 8, 5);
			RECT update = {(gr.bounding->left + gr.bounding->right) / 2, gr.bounding->bottom + 1, gr.bounding->right, gr.bounding->bottom + gr.grybuff };
			InvalidateRect(hWnd, &update, false);
		}
		break;
	}
	case WM_LBUTTONUP:
		hold = false;
		break;
	case WM_MOUSEWHEEL:
	{
		return 0;

		float wDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		wDelta /= 1;
		gr.upperx /= wDelta;
		gr.lowerx /= wDelta;
		gr.uppery /= wDelta;
		gr.lowery /= wDelta;
		InvalidateRect(hWnd, NULL, false);
		return 0;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 'i':
			gr.upperx /= 1.1;
			gr.uppery /= 1.1;
			gr.lowerx /= 1.1;
			gr.lowery /= 1.1;
			InvalidateRect(hWnd, NULL, false);
			break;
		case 'o':
			gr.upperx *= 1.1;
			gr.uppery *= 1.1;
			gr.lowerx *= 1.1;
			gr.lowery *= 1.1;
			InvalidateRect(hWnd, NULL, false);
			break;
		default:
			break;
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
			gr.upperx -= 1;
			gr.lowerx -= 1;
			InvalidateRect(hWnd, NULL, false);
			break;
		case VK_UP:
			gr.uppery += 1;
			gr.lowery += 1;
			InvalidateRect(hWnd, NULL, false);
			break;
		case VK_DOWN:
			gr.uppery -= 1;
			gr.lowery -= 1;
			InvalidateRect(hWnd, NULL, false);
			break;
		case VK_RIGHT:
			gr.upperx += 1;
			gr.lowerx += 1;
			InvalidateRect(hWnd, NULL, false);
			break;
		}
		return 0;
	}
	case WM_CLOSE:
		freeFunc(f1, 0);
		ShowWindow(hWndPare, SW_SHOW);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}