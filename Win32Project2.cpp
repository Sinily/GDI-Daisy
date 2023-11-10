#include "stdafx.h"
#include "Win32Project2.h"
#include <thread>
#include <chrono>

#define MAX_LOADSTRING 100
#define ID_TIMER 1
#define TIMER_INTERVAL 333

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
INT ak_max = 13;								// max value for AK										<- petal amount

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

VOID DAISY(HDC hdc, int ak, int xend, int yend)
{
	// Drawing code goes here:
	Graphics graphics(hdc);

	int dx = xend / 2;
	int dy = yend / 2 + 215;


	// element coordinates
	Point stemPoints[] = {
		Point(dx + 50, dy + 50),	// bottom of stem
		Point(dx + 50, dy - 50),
		Point(dx + 50, dy - 200),
		Point(dx + 2, dy - 300)		// top of stem
	};
	Point petalPoints[] = {
		Point(dx - 1, dy - 320),
		Point(dx - (666 / ak_max), dy - 444),
		Point(dx + (666 / ak_max), dy - 444),
		Point(dx + 1, dy - 320)
	};
	Point leafPoints[] = {
		Point(dx + 53, dy + 40),
		Point(dx + 90, dy - 9),
		Point(dx + 110, dy - 29),
		Point(dx + 90, dy - 119)
	};
	Point leafPointsMirror[] = {
		Point(dx + 53, dy + 40),
		Point(dx + 50, dy - 19),
		Point(dx + 60, dy - 69),
		Point(dx + 90, dy - 119)
	};


	// brushes
	SolidBrush backgroundBrush(Color(255, 209, 237, 242)); // light_blue
	SolidBrush stemBrush(Color(255, 0, 255, 0));		   // green
	SolidBrush petalBrush(Color(255, 255, 255, 255));	   // white
	SolidBrush circleBrush(Color(255, 255, 255, 0));	   // yellow


	// background
	Rect blueRect(0, 0, xend, yend);
	graphics.FillRectangle(&backgroundBrush, blueRect);


	// stem
	GraphicsPath stemPath;
	stemPath.StartFigure();
	stemPath.AddBeziers(stemPoints, 4);
	graphics.FillPath(&stemBrush, &stemPath);


	// leaf
	GraphicsPath leafPath;
	leafPath.StartFigure();
	leafPath.AddBeziers(leafPoints, 4);
	leafPath.AddBeziers(leafPointsMirror, 4);
	graphics.FillPath(&stemBrush, &leafPath);


	// circle
	GraphicsPath circlePath;
	circlePath.AddEllipse(dx - 25, dy - 325, 50, 50);
	graphics.FillPath(&circleBrush, &circlePath);


	// petals
	for (int i = 0; i < ak; i++) 
	{
		// angle for each petal transformation
		float petalAngle = ((360.0f / ak_max) * i) - 182.5f;
		Matrix transformationMatrix;
		transformationMatrix.RotateAt(petalAngle, PointF(dx, dy - 300));

		// placing petals
		GraphicsPath petalPath;
		petalPath.StartFigure();
		petalPath.AddBeziers(petalPoints, 4);

		// rotating / transforming
		petalPath.Transform(&transformationMatrix);

		// applying color
		graphics.FillPath(&petalBrush, &petalPath);
	}
}
int ak = 0; // counter

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32PROJECT2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
		break;
	case WM_TIMER:
		if (wParam == ID_TIMER) {
			// Handle the timer event
			ak++;
			if (ak >= ak_max) KillTimer(hWnd, ID_TIMER);
			InvalidateRect(hWnd, NULL, true);
			::UpdateWindow(hWnd);
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		RECT rt;
		GetClientRect(hWnd, &rt);
		DAISY(hdc, ak, int(rt.right), int(rt.bottom));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}