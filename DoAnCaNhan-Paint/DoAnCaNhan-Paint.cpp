// DoAnCaNhan-Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DoAnCaNhan-Paint.h"
#include "resource.h"

#pragma comment(lib, "Gdi32.lib")
using namespace PaintLibrary;

#define MAX_LOADSTRING 100
#define MAX_BUFF 255

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;
HDC hdc;
// vector chứa các đối tượng đã vẽ
vector<CShape*> shapes;
vector<CShape*> temp;
// đối tượng đang vẽ
CShape* currentShape;
CShape* pointer;
vector<CShape*> pointerUp;
CShape* pointer2;
// loại hình đang vẽ
int selectedShape = 0;
// có đang vẽ
bool isDrawing = FALSE;
bool pointerSelected = FALSE;
// toạ độ khi nhấn chuột và hiện tại
int firstX;
int firstY;
int lastX;
int lastY;

COLORREF currBrush = -1;
int currPenStyle = PS_SOLID;
int currPenSize = 1;
COLORREF currColor = RGB(0, 0, 0);
int width = 1024;
int height = 768;

WNDCLASSEXW wcex;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DOANCANHANPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DOANCANHANPAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{


    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0; /*CS_HREDRAW | CS_VREDRAW;*/
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DOANCANHANPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DOANCANHANPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   0, 0, width, height, NULL, NULL, hInstance, NULL);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case ID_SAVEAS_BITMAP: ExportToPictureFile(hWnd, 0); break;
		case ID_SAVEAS_PNG: ExportToPictureFile(hWnd, 1); break;
		case ID_SAVEAS_JPG: ExportToPictureFile(hWnd, 2); break;

		case ID_FILE_EXPORT: Export(shapes, "Data.txt"); break;
		case ID_FILE_IMPORT: Import("Data.txt"); break;

		case ID_EDIT_UNDO: Undo(); break;
		case ID_EDIT_REDO: Redo(); break;
		case ID_EDIT_COPY: if (pointer != NULL || pointer2 != NULL) OnCopy(); break;

		case ID_TOOL_POINTER:  pointerSelected = TRUE; break;
		case ID_DRAW_LINE32793: selectedShape = LINE; pointerSelected = FALSE; break;
		case ID_DRAW_RECTANGLE32794: selectedShape = RECTANGLE; pointerSelected = FALSE; break;
		case ID_DRAW_SQUARE32795: selectedShape = SQUARE; pointerSelected = FALSE; break;
		case ID_DRAW_ELLIPSE32796: selectedShape = ELLIPSE; pointerSelected = FALSE; break;
		case ID_DRAW_CIRCLE32797: selectedShape = CIRCLE; pointerSelected = FALSE; break;
		case ID_DRAW_ARROW: selectedShape = ARROW;  break;
		case ID_DRAW_STAR: selectedShape = STAR; break;

		case ID_STROKES_SOLID: currPenStyle = PS_SOLID; break;
		case ID_STROKES_DOT: currPenStyle = PS_DOT; break;
		case ID_STROKES_DASH: currPenStyle = PS_DASH; break;
		
		case ID_COLOR_BLACK: currColor = RGB(0, 0, 0); break;
		case ID_COLOR_RED: currColor = RGB(219, 50, 54); break;
		case ID_COLOR_GREEN: currColor = RGB(60, 186, 84); break;
		case ID_COLOR_BLUE:currColor = RGB(72, 133, 237); break;
		case ID_COLOR_YELLOW: currColor = RGB(244, 194, 13); break;

		case ID_FILLCOLOR_TRANSPARENT: currBrush = -1; break;
		case ID_FILLCOLOR_BLACK: currBrush = BLACK_BRUSH; break;
		case ID_FILLCOLOR_RED: currBrush = RGB(219, 50, 54); break;
		case ID_FILLCOLOR_GREEN: currBrush = RGB(60, 186, 84); break;
		case ID_FILLCOLOR_BLUE: currBrush = RGB(72, 133, 237); break;
		case ID_FILLCOLOR_YELLO: currBrush = RGB(244, 194, 13); break;

		case ID_SIZE_1: currPenSize = 1; break;
		case ID_SIZE_2: currPenSize = 2; break;
		case ID_SIZE_3: currPenSize = 3; break;
		case ID_SIZE_4: currPenSize = 4; break;
		case ID_SIZE_5: currPenSize = 5; break;


		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		pointer = NULL;
		pointer2 = NULL;
		InvalidateRect(hWnd, NULL, TRUE);
		firstX = GET_X_LPARAM(lParam);
		firstY = GET_Y_LPARAM(lParam);
		if (!isDrawing) {
			isDrawing = TRUE;

			if (pointerSelected == FALSE)
			{
				switch (selectedShape)
				{
				case LINE:
					currentShape = new CLine();
					break;
				case RECTANGLE:
					currentShape = new CRectangle();
					break;
				case SQUARE:
					currentShape = new CSquare();
					break;
				case ELLIPSE:
					currentShape = new CEllipse();
					break;
				case CIRCLE:
					currentShape = new CCircle();
					break;
				case ARROW:
					currentShape = new CArrow();
					break;
				case STAR:
					currentShape = new CStar();
					break;
				}
				currentShape->SetType(selectedShape);
				currentShape->SetData(firstX, firstY, firstX, firstY, currColor, currPenStyle, currPenSize);
				currentShape->SetBrush(currBrush);
			}
			else
			{
				pointer = new CRectangle();
				pointer->SetType(RECTANGLE);
				pointer->SetData(firstX, firstY, firstX, firstY, RGB(0, 0, 0), PS_DASH, 1);
			}
		}
		break;

	case WM_MOUSEMOVE:
		if (isDrawing) {
			lastX = GET_X_LPARAM(lParam);
			lastY = GET_Y_LPARAM(lParam);
			if (pointerSelected == FALSE)
			{
				currentShape->SetData(firstX, firstY, lastX, lastY, currColor, currPenStyle, currPenSize);
				currentShape->SetBrush(currBrush);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
			{
				pointer->SetData(firstX, firstY, lastX, lastY, RGB(0, 0, 0), PS_DASH, 1);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			
		}
		break;

	case WM_LBUTTONUP:
		if (isDrawing) {
			if (pointerSelected == FALSE)
			{
				shapes.push_back(currentShape);
				isDrawing = FALSE;
				temp.clear();
				
				
			}
			else
			{
				pointer2 = pointer;
				isDrawing = FALSE;
				
			}
			InvalidateRect(hWnd, NULL, TRUE);
			
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		
		HDC hdcMem;
		HBITMAP hMap;
		hdc = BeginPaint(hWnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		hMap = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top);
		HANDLE hold = SelectObject(hdcMem, hMap);
		FillRect(hdcMem, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
		//SelectObject(hdcMem, GetStockObject(DC_PEN));

		

		// vẽ lại danh sách hình trước đó
		if (shapes.size() > 0)
		{
			for (int i = 0; i < shapes.size(); i++) {
				HPEN hPen = CreatePen(shapes[i]->GetPenStyle(), shapes[i]->GetPenSize(), shapes[i]->GetColor());
				SelectObject(hdcMem, hPen);
				if (shapes[i]->GetBrush() != -1)
				{
					SelectObject(hdcMem, GetStockObject(DC_BRUSH));
					SetDCBrushColor(hdcMem, shapes[i]->GetBrush());
				}
				else
					SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
				shapes[i]->Draw(hdcMem);
				//if (shapes[i]->isFilled)
				//	shapes[i]->Fill(hdcMem);
			}
			// vẽ lại hình hiện tại
			if (isDrawing) {
				HPEN hPen = CreatePen(currentShape->GetPenStyle(), currentShape->GetPenSize(), currentShape->GetColor());
				SelectObject(hdcMem, hPen);
				if (currentShape->GetBrush() != -1)
				{
					SelectObject(hdcMem, GetStockObject(DC_BRUSH));
					SetDCBrushColor(hdcMem, currentShape->GetBrush());
				}
				else
					SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
				currentShape->Draw(hdcMem);
			}
			BitBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hdcMem, 0, 0, SRCCOPY);
			SelectObject(hdcMem, hold);
		}

		// vẽ pointer chọn vùng
		if (pointer2 != NULL)
		{
			HPEN hPen = CreatePen(pointer2->GetPenStyle(), pointer2->GetPenSize(), pointer2->GetColor());
			SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			pointer2->Draw(hdc);
			
		}
		//if (pointer != NULL && isDrawing)
		//{
		//	HPEN hPen = CreatePen(pointer->GetPenStyle(), pointer->GetPenSize(), pointer->GetColor());
		//	SelectObject(hdc, hPen);
		//	SelectObject(hdc, GetStockObject(NULL_BRUSH));
		//	pointer->Draw(hdc);
		//}
		

		DeleteObject(hMap);
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_ERASEBKGND:
	{
		return 1;
	}
	break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hWnd, NULL, FALSE);
			//UpdateWindow(hWnd);
			//ReleaseDC(hwnd, hdc);
			return 0;
		}
		break;
	case WM_DESTROY:
	{

		PostQuitMessage(0);
	}
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


void Export(vector<CShape*> shapes, string filename)
{
	fstream fout;
	fout.open(filename, ios::out);

	fout << shapes.size() << "\n";
	for (int i = 0; i < shapes.size(); i++) 
	{
		int type = shapes[i]->GetType();
		COLORREF color = shapes[i]->GetColor();
		COLORREF colorBrush = shapes[i]->GetBrush();
		RECT* rect = shapes[i]->GetRect();
		int style = shapes[i]->GetPenStyle();
		int size = shapes[i]->GetPenSize();

		fout << type << " ";
		fout << (int)GetRValue(color) << " ";
		fout << (int)GetGValue(color) << " ";
		fout << (int)GetBValue(color) << " ";
		fout << (int)GetRValue(colorBrush) << " ";
		fout << (int)GetGValue(colorBrush) << " ";
		fout << (int)GetBValue(colorBrush) << " ";
		fout << rect->left << " ";
		fout << rect->top << " ";
		fout << rect->right << " ";
		fout << rect->bottom << " ";
		fout << style << " ";
		fout << size << "\n";

	}
	fout.close();

}

void Import(string filename)
{
	fstream fin;

	fin.open(filename, ios::in);
	CShape* shape;
	int size;
	fin >> size;
	shapes.clear();

	if (fin.fail())
	{
		MessageBox(hWnd, L"File not existed!", L"Error", MB_OK);
		return;
	}
		
	for (int i = 0; i < size; i++)
	{
		int type, style, size, r, g, b, r1, g1, b1;
		COLORREF color, colorBrush;
		RECT rect;
		fin >> type;
		fin >> r >> g >> b;
		color = RGB(r, g, b);
		fin >> r1 >> g1 >> b1;
		colorBrush = RGB(r1, g1, b1);
		fin >> rect.left >> rect.top >> rect.right >> rect.bottom;
		fin >> style >> size;

		switch (type)
		{
		case LINE:
			shape = new CLine();
			break;
		case RECTANGLE:
			shape = new CRectangle();
			break;
		case SQUARE:
			shape = new CSquare();
			break;
		case ELLIPSE:
			shape = new CEllipse();
			break;
		case CIRCLE:
			shape = new CCircle();
			break;
		case ARROW:
			shape = new CArrow();
			break;
		case STAR:
			shape = new CStar();
			break;
		}
		shape->SetType(type);
		shape->SetData(rect.left, rect.top, rect.right, rect.bottom, color, style, size);
		shape->SetBrush(colorBrush);



		shapes.push_back(shape);

	}

	InvalidateRect(hWnd, NULL, TRUE);
	fin.close();
}


void Undo()
{
	if (shapes.size() > 0)
	{
		temp.push_back(shapes.back());

		shapes.pop_back();
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

void Redo()
{
	if (temp.size() > 0)
	{
		shapes.push_back(temp.back());
		temp.pop_back();
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

void OnCopy() 
{
	
	// copy screen to bitmap
	HDC     hScreen = GetDC(hWnd);
	HDC     hDC = CreateCompatibleDC(hScreen);
	int width = abs(pointer2->GetX2() - pointer2->GetX1());
	int height = abs(pointer2->GetY2() - pointer2->GetY1());
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width,height);
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, width, height, hScreen, pointer2->GetX1(), pointer2->GetY1(), SRCCOPY);

	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}
