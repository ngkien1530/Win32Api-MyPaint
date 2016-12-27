#pragma once

#ifdef PAINTLIB_EXPORTS 
#define PAINTLIB_API __declspec(dllexport) 
#else
#define PAINTLIB_API __declspec(dllimport) 
#endif 

#include <iostream>

namespace PaintLibrary {
	class CShape {
	public:
		int penSize;
		COLORREF colorBrush;
		COLORREF color;
		int penStyle;
		int shapeType;
		int x1;
		int y1;
		int x2;
		int y2;
	public:
		PAINTLIB_API virtual void Draw(HDC hdc) = 0;
		PAINTLIB_API virtual CShape* Create() = 0;
		PAINTLIB_API virtual void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) = 0;
		PAINTLIB_API void SetBrush(COLORREF colorBrush) { this->colorBrush = colorBrush; }
		PAINTLIB_API COLORREF GetBrush() { return colorBrush; }
		PAINTLIB_API void SetType(int type) { this->shapeType = type; }
		PAINTLIB_API void SetPenStyle(int penStyle) { this->penStyle = penStyle; }
		PAINTLIB_API void SetPenSize(int penSize) { this->penSize = penSize; }
		PAINTLIB_API void SetColor(COLORREF color) { this->color = color; }
		PAINTLIB_API int GetX1() { return x1; }
		PAINTLIB_API int GetX2() { return x2; }
		PAINTLIB_API int GetY1() { return y1; }
		PAINTLIB_API int GetY2() { return y2; }
		PAINTLIB_API int GetType() { return shapeType; }
		PAINTLIB_API int GetPenStyle() { return penStyle; };
		PAINTLIB_API COLORREF GetColor() { return color; };
		PAINTLIB_API int GetPenSize() { return penSize; };
		PAINTLIB_API RECT* GetRect()
		{
			RECT* rect = new RECT;
			rect->left = x1;
			rect->top = y1;
			rect->right = x2;
			rect->bottom = y2;
			return rect;
		}

	};

	class  CLine : public CShape {

	public:
		PAINTLIB_API void Draw(HDC hdc) {
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2, y2);
		}

		PAINTLIB_API CShape* Create() { return new CLine; }

		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CRectangle : public CShape {

	public:
		PAINTLIB_API void Draw(HDC hdc) {
			Rectangle(hdc, x1, y1, x2, y2);
		}

		PAINTLIB_API CShape* Create() { return new CRectangle; }
		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CSquare : public CShape {

	public:
		PAINTLIB_API void Draw(HDC hdc) {
			int xdiff = abs(x2 - x1);
			int ydiff = abs(y2 - y1);
			int left, top, right, bottom;
			left = x1;
			top = y1;
			right = x2;
			bottom = y2;
			if (xdiff > ydiff)
			{
				if (y2 > y1)
					bottom = y1 + xdiff;
				else
					bottom = y1 - xdiff;
			}
			else
			{
				if (x2 > x1)
					right = x1 + ydiff;
				else
					right = x1 - ydiff;
			}
			Rectangle(hdc, x1, y1, right, bottom);

		}

		PAINTLIB_API CShape* Create() { return new CSquare; }
		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CEllipse : public CShape {

	public:
		PAINTLIB_API void Draw(HDC hdc) {
			Ellipse(hdc, x1, y1, x2, y2);

		}

		PAINTLIB_API CShape* Create() { return new CEllipse; }
		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CCircle : public CShape {

	public:
		PAINTLIB_API void Draw(HDC hdc) {
			int xdiff = abs(x2 - x1);
			int ydiff = abs(y2 - y1);
			int left, top, right, bottom;
			left = x1;
			top = y1;
			right = x2;
			bottom = y2;
			if (xdiff > ydiff)
			{
				if (y2 > y1)
					bottom = y1 + xdiff;
				else
					bottom = y1 - xdiff;
			}
			else
			{
				if (x2 > x1)
					right = x1 + ydiff;
				else
					right = x1 - ydiff;
			}
			Ellipse(hdc, x1, y1, right, bottom);

		}

		PAINTLIB_API CShape* Create() { return new CCircle; }
		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CArrow :public CShape
	{
	public:
		PAINTLIB_API void Draw(HDC hdc)
		{
			MoveToEx(hdc, 0.15*x1 + 0.75*x2, y1, NULL);
			LineTo(hdc, x2, 0.5*y1 + 0.5*y2);

			MoveToEx(hdc, 0.15*x1 + 0.75*x2, y2, NULL);
			LineTo(hdc, x2, 0.5*y1 + 0.5*y2);

			MoveToEx(hdc, 0.15*x1 + 0.75*x2, y1, NULL);
			LineTo(hdc, 0.15*x1 + 0.75*x2, 0.7*y1 + 0.3*y2);
			
			MoveToEx(hdc, 0.15*x1 + 0.75*x2, y2, NULL);
			LineTo(hdc, 0.15*x1 + 0.75*x2, 0.7*y2 + 0.3*y1);

			MoveToEx(hdc, 0.15*x1 + 0.75*x2, 0.7*y1 + 0.3*y2, NULL);
			LineTo(hdc, x1, 0.7*y1 + 0.3*y2);

			MoveToEx(hdc, 0.15*x1 + 0.75*x2, 0.7*y2 + 0.3*y1, NULL);
			LineTo(hdc, x1, 0.7*y2 + 0.3*y1);

			MoveToEx(hdc, x1, 0.7*y2 + 0.3*y1, NULL);
			LineTo(hdc, x1, 0.7*y1 + 0.3*y2);
		}
		PAINTLIB_API CShape* Create() { return new CArrow; }

		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};

	class CStar :public CShape
	{
	public:
		PAINTLIB_API void Draw(HDC hdc)
		{
			MoveToEx(hdc, 0.5*x1 + 0.5*x2, y1, NULL);
			LineTo(hdc, 0.62*x1 + 0.38*x2, 0.6*y1 + 0.4*y2);
			MoveToEx(hdc, 0.62*x1 + 0.38*x2, 0.6*y1 + 0.4*y2, NULL);
			LineTo(hdc, x1, 0.6*y1 + 0.4*y2);
			MoveToEx(hdc, x1, 0.6*y1 + 0.4*y2, NULL);
			LineTo(hdc, 0.69*x1 + 0.31*x2, 0.38*y1 + 0.62*y2);
			MoveToEx(hdc, 0.69*x1 + 0.31*x2, 0.38*y1 + 0.62*y2, NULL);
			LineTo(hdc, 0.8*x1 + 0.2*x2, y2);
			MoveToEx(hdc, 0.8*x1 + 0.2*x2, y2, NULL);
			LineTo(hdc, 0.5*x1 + 0.5*x2, 0.23*y1 + 0.77*y2);
			MoveToEx(hdc, 0.5*x1 + 0.5*x2, 0.23*y1 + 0.77*y2, NULL);
			LineTo(hdc, 0.2*x1 + 0.8*x2, y2);
			MoveToEx(hdc, 0.2*x1 + 0.8*x2, y2, NULL);
			LineTo(hdc, 0.31*x1 + 0.69*x2, 0.38*y1 + 0.62*y2);
			MoveToEx(hdc, 0.31*x1 + 0.69*x2, 0.38*y1 + 0.62*y2, NULL);
			LineTo(hdc, x2, 0.6*y1 + 0.4*y2);
			MoveToEx(hdc, x2, 0.6*y1 + 0.4*y2, NULL);
			LineTo(hdc, 0.38*x1 + 0.62*x2, 0.6*y1 + 0.4*y2);
			MoveToEx(hdc, 0.38*x1 + 0.62*x2, 0.6*y1 + 0.4*y2, NULL);
			LineTo(hdc, 0.5*x1 + 0.5*x2, y1);

		}
		PAINTLIB_API CShape* Create() { return new CStar; }

		PAINTLIB_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize) {
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
			this->penSize = penSize;
			this->color = color;
			this->penStyle = penStyle;
		}
	};
}



