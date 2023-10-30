
#pragma once
#include <windows.h> // win32 api
#include <windowsx.h> // standard windows controls 
#include <stdlib.h> // standard library
#include <string.h> // standard string library
#include <tchar.h> // fancy strings
#include <corecrt_math.h> // math

#include <iostream> // io
#include <fstream> // file io
#include <chrono> // time

//#include "MathInputLibrary.h" // hopefully a dll?

#include "resource.h" // all the resource files
#include "NaturalMathInputHeader.hpp" //nmi input

#define windowGraph "WindowsDesktopGraph1"


struct GraphProp {
	int grxbuff; int grybuff;
	float lowert; float uppert; float tres; int isPara;
	float lowerx; float upperx;
	float lowery; float uppery;
	RECT* bounding;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndGraphProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef float winEqua[7];

int intToChar(int, TCHAR*, int);
int charToInt(TCHAR*, int);
int fileToInt(char*, int);
int floatToChar(float, TCHAR*, int);

int updateClick(int, HWND);
int loadClick(char*);
int saveClick(char*, int);

float toFunc(winEqua, float);
int updateWindowBounds(HWND, GraphProp);
int updateAxis(HDC hdc, GraphProp gr);
int updateGraph(HDC hdc, float (*funcx)(float), float (*funcy)(float), GraphProp);
int updateNMIGraph(HDC hdc, struct Func* f1, GraphProp gr);
int updateDFGraph(HDC hdc, float (*func)(float, float), float, float, GraphProp);
float toPixel(float inp, GraphProp gr, bool isX);
float toAxis(float inp, GraphProp gr, bool isX);
COLORREF chroma(int);
