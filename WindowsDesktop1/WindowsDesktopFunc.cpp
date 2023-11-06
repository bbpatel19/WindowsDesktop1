#include "WindowsDesktop1.h"

int intToChar(int input, TCHAR* output, int length) {
	for (int i = 0; i < length; i++) {
		output[i] = wchar_t((input % int(pow(10, length - i)) / int(pow(10, length - i - 1))) + 48);
	}
	return 0;
}

int charToInt(TCHAR* input, int length) {
	int summ = 0;
	for (int i = 0; i < length; i++) {
		if (input[i] < 48 || input[i] > 57) return -1;
		summ += (input[i] - 48) * (int)pow(10, length - i - 1);
	}
	return summ;
}

int fileToInt(char* input, int length) {
	int summ = 0;
	for (int i = 0; i < length; i++) {
		if (input[i] == 0) return summ / pow(10, length - i);
		if (input[i] < 48 || input[i] > 57) return -1;
		summ += (input[i] - 48) * (int)pow(10, length - i - 1);
	}
	return summ;
}

int floatToChar(float inputF, TCHAR* output, int length) {

	if (abs(inputF) > 9999999 || abs(inputF) < 0.0000001) { return -1; }
	TCHAR temp[] = _T("0000000000000000");

	int integer = abs(inputF);
	int frac = abs((inputF - integer) * 10000000);

	temp[0] = (wchar_t)43;
	if (inputF < 0) { temp[0] = (wchar_t)45; }

	intToChar(integer, temp + 1, 7);
	temp[8] = (wchar_t)46;
	intToChar(frac, temp + 9, 7);

	output[0] = temp[0];

	bool numberYet = false;
	int j = 1;
	for (int i = 1; i < 16; i++) {
		if (temp[i] == '0' && !numberYet) { continue; }
		output[j] = temp[i];
		numberYet = true;
		j++;
		if (j > length - 1) break;
	}
	return 0;
}

int updateClick(int numberclick, HWND hWnd) {
	TCHAR clicks[] = _T("000000");
	intToChar(numberclick, clicks, _tcslen(clicks));
	HDC hdc = GetDC(hWnd);
	//TextOut(hdc, 5, 15, greeting, _tcslen(greeting));
	TextOut(hdc, 50, 50, clicks, _tcslen(clicks));
	ReleaseDC(hWnd, hdc);
	return 0;
}

int loadClick(char* saveFile) {
	std::ifstream myfile(saveFile);
	char saveClicked[] = "000000";
	if (myfile.is_open()) {
		myfile >> saveClicked;
	}
	myfile.close();
	int clicksProc = fileToInt(saveClicked, 6);
	if (clicksProc < 0) {
		MessageBox(NULL, _T("Enter a number!"), _T("Windows Desktop Application"), NULL);
	}
	return clicksProc;
}

int saveClick(char* saveFile, int clicked) {
	std::ofstream myfile;
	myfile.open(saveFile);
	myfile << clicked;
	myfile.close();
	return 0;
}



COLORREF chroma(int t) {
	COLORREF rainbow[12] = {
		RGB(255,   0,   0),
		RGB(255, 128,   0),
		RGB(255, 255,   0),
		RGB(128, 255,   0),
		RGB(  0, 255,   0),
		RGB(  0, 255, 128),
		RGB(  0, 255, 255),
		RGB(  0, 128, 255),
		RGB(  0,   0, 255),
		RGB(128,   0, 255),
		RGB(255,   0, 255),
		RGB(255,   0, 128),
	};
	return rainbow[t % 12];
}

float toFunc(winEqua input, float x) {
	// winEqua has length 7
	// x^0 x^1 x^2 x^3 sin(x) unused unused
	return input[0] + x * input[1] + x * x * input[2] + x * x * x * input[3] + sin(input[4] * x);
}

int updateWindowBounds(HWND hWnd, GraphProp gr) {
	GetClientRect(hWnd, gr.bounding);
	gr.bounding->right -= gr.grxbuff;
	gr.bounding->left += gr.grxbuff;
	gr.bounding->bottom -= gr.grybuff;
	gr.bounding->top += gr.grybuff;
	return 0;
}

float toAxis(float inp, GraphProp gr, bool isX) {
	float output = 0;

	if (isX) {
		float xrange = gr.upperx - gr.lowerx;
		float MAXwidth = gr.bounding->right - gr.bounding->left;
		output = (inp - gr.bounding->left + MAXwidth / xrange * gr.lowerx) * xrange / MAXwidth;
		return output;
	}

	float yrange = gr.uppery - gr.lowery;
	float MAXheight = gr.bounding->bottom - gr.bounding->top;
	output = -(inp - gr.bounding->top - MAXheight / yrange * gr.uppery) * yrange / MAXheight;

	return output;
}

float toPixel(float inp, GraphProp gr, bool isX) {
	float output = 0;
	
	if (isX) {
		float xrange = gr.upperx - gr.lowerx;
		float MAXwidth = gr.bounding->right - gr.bounding->left;
		output = (inp * MAXwidth / xrange) - MAXwidth / xrange * gr.lowerx;
		return output + gr.bounding->left;
	}

	float yrange = gr.uppery - gr.lowery;
	float MAXheight = gr.bounding->bottom - gr.bounding->top;
	output = -(inp * MAXheight / yrange) + (MAXheight / yrange * gr.uppery);
	return output + gr.bounding->top;
}

int updateAxis(HDC hdc, GraphProp gr) {
	int x0 = toPixel(0, gr, true);
	int y0 = toPixel(0, gr, false);

	MoveToEx(hdc, gr.bounding->left, gr.bounding->top, (LPPOINT)NULL);
	LineTo(hdc, gr.bounding->left, gr.bounding->bottom);
	LineTo(hdc, gr.bounding->right, gr.bounding->bottom);
	LineTo(hdc, gr.bounding->right, gr.bounding->top);
	LineTo(hdc, gr.bounding->left, gr.bounding->top);

	if (y0 < gr.bounding->bottom && y0 > gr.bounding->top) {
		MoveToEx(hdc, gr.bounding->right, y0, (LPPOINT)NULL);
		LineTo(hdc, gr.bounding->left, y0);
	}

	if (x0 < gr.bounding->right && x0 > gr.bounding->left) {
		MoveToEx(hdc, x0, gr.bounding->top, (LPPOINT)NULL);
		LineTo(hdc, x0, gr.bounding->bottom);
	}
	return 0;
}

int updateGraph(HDC hdc, float (*funcx)(float), float (*funcy)(float), GraphProp gr) {
	float tres = gr.tres;
	if (gr.isPara) {
		if (gr.isPara == -1) {
			gr.uppert = gr.uppery;
			gr.lowert = gr.lowery;
		}
		else {
			gr.uppert = gr.upperx;
			gr.lowert = gr.lowerx;
		}
	}
	tres *= (gr.uppert - gr.lowert);
	for (float t = gr.lowert; t < gr.uppert; t = t + tres) {
		float px1 = toPixel(funcx(t), gr, true);
		float py1 = toPixel(funcy(t), gr, false);
		float px2 = toPixel(funcx(t + tres), gr, true);
		float py2 = toPixel(funcy(t + tres), gr, false);

		if (py2 > gr.bounding->bottom && (py1 > gr.bounding->bottom || py1 < gr.bounding->top)) continue;
		if (py2 < gr.bounding->top && (py1 < gr.bounding->top || py1 > gr.bounding->bottom)) continue;
		if (px1 > gr.bounding->right && (px2 > gr.bounding->right || px1 < gr.bounding->left)) continue;
		if (px1 < gr.bounding->left && (px2 < gr.bounding->left || px1 > gr.bounding->right)) continue;

		py2 = max(min(py2, gr.bounding->bottom), gr.bounding->top);
		py1 = max(min(py1, gr.bounding->bottom), gr.bounding->top);
		px1 = max(min(px1, gr.bounding->right), gr.bounding->left);
		px2 = max(min(px2, gr.bounding->right), gr.bounding->left);
		
		MoveToEx(hdc, px1, py1, (LPPOINT)NULL);
		LineTo(hdc, px2, py2);
	}
	return 0;
}

int updateNMIGraph(HDC hdc, struct Func * f1, GraphProp gr) {
	float tres = gr.tres;
	if (gr.isPara) {
		if (gr.isPara == -1) {
			gr.uppert = gr.uppery;
			gr.lowert = gr.lowery;
		}
		else {
			gr.uppert = gr.upperx;
			gr.lowert = gr.lowerx;
		}
	}
	tres *= (gr.uppert - gr.lowert);
	for (float t = gr.lowert; t < gr.uppert; t = t + tres) {
		float px1 = toPixel(t, gr, true);
		float py1 = toPixel(evalFunct(f1, t), gr, false);
		float px2 = toPixel(t + tres, gr, true);
		float py2 = toPixel(evalFunct(f1, t + tres), gr, false);

		if (py2 > gr.bounding->bottom && (py1 > gr.bounding->bottom || py1 < gr.bounding->top)) continue;
		if (py2 < gr.bounding->top && (py1 < gr.bounding->top || py1 > gr.bounding->bottom)) continue;
		if (px1 > gr.bounding->right && (px2 > gr.bounding->right || px1 < gr.bounding->left)) continue;
		if (px1 < gr.bounding->left && (px2 < gr.bounding->left || px1 > gr.bounding->right)) continue;

		py2 = max(min(py2, gr.bounding->bottom), gr.bounding->top);
		py1 = max(min(py1, gr.bounding->bottom), gr.bounding->top);
		px1 = max(min(px1, gr.bounding->right), gr.bounding->left);
		px2 = max(min(px2, gr.bounding->right), gr.bounding->left);

		MoveToEx(hdc, px1, py1, (LPPOINT)NULL);
		LineTo(hdc, px2, py2);
	}
	return 0;
}

int updateDFGraph(HDC hdc, float (*func)(float, float), float inix, float iniy, GraphProp gr) {

	float curry = iniy;
	float tres = 1;

	for (int i = toPixel(inix, gr, true); i < gr.bounding->right; i = i + tres) {

		float px1 = i;
		float py1 = toPixel(curry, gr, false);
		float px2 = i + tres;
		float nexty = func(toAxis(px2, gr, true), curry) * tres * (gr.upperx - gr.lowerx) / (gr.bounding->right - gr.bounding->left) + curry;
		float py2 = toPixel(nexty, gr, false);
		curry = nexty;

		if (py2 > gr.bounding->bottom && (py1 > gr.bounding->bottom || py1 < gr.bounding->top)) continue;
		if (py2 < gr.bounding->top && (py1 < gr.bounding->top || py1 > gr.bounding->bottom)) continue;
		if (px1 > gr.bounding->right && (px2 > gr.bounding->right || px1 < gr.bounding->left)) continue;
		if (px1 < gr.bounding->left && (px2 < gr.bounding->left || px1 > gr.bounding->right)) continue;

		py2 = max(min(py2, gr.bounding->bottom), gr.bounding->top);
		py1 = max(min(py1, gr.bounding->bottom), gr.bounding->top);
		px1 = max(min(px1, gr.bounding->right), gr.bounding->left);
		px2 = max(min(px2, gr.bounding->right), gr.bounding->left);

		MoveToEx(hdc, px1, py1, (LPPOINT)NULL);
		LineTo(hdc, px2, py2);
	}
	return 0;
}

int plotNMIpoint(HDC hdc, nmi_point points[], int length, int size, COLORREF col,  GraphProp gr) {

	float xPix = 0;
	float yPix = 0;
	for (int i = 0; i < length; i++) {
		xPix = toPixel(points[i].x, gr, true);
		yPix = toPixel(points[i].y, gr, false);


		for (int j = -size; j <= size; j++) {
			for (int k = -size; k <= size; k++) {
				if ((xPix + j) < gr.bounding->left || (xPix + j) > gr.bounding->right) continue;
				if ((yPix + k) < gr.bounding->top || (yPix + k) > gr.bounding->bottom) continue;
				SetPixel(hdc, xPix + j, yPix + k, col);
			}
		}
	}

	return 0;
}