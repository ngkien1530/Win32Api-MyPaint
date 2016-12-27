#include "DoAnCaNhan-Paint.h"

#include "stdafx.h"

using namespace std;
#include <Unknwn.h>
#include <windows.h>
#include <gdiplus.h>
#include <objidl.h>
using namespace Gdiplus;
 
#pragma comment (lib,"Gdiplus.lib")

INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{

	UINT  num = 0;            // number of image encoders
	UINT  size = 0;           // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)malloc(size);
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;                                         // Success
		}
	}
	free(pImageCodecInfo);
	return -1;
}


PBITMAPINFO CreateBitmapInfo(HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;

	GetObject(hBmp, sizeof(BITMAP), &bmp);

	pbmi = static_cast<PBITMAPINFO>(LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; // we are assuming that there is only one plane
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

	// no compression this is an rgb bitmap
	pbmi->bmiHeader.biCompression = BI_RGB;

	// calculate size and align to a DWORD (8bit), we are assuming there is only one plane.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * bmp.bmBitsPixel + 31) & -31) * pbmi->bmiHeader.biHeight;

	// all device colours are important
	pbmi->bmiHeader.biClrImportant = 0;

	return pbmi;
}

bool SaveBMPFile(LPCTSTR filename, HBITMAP hBitmap, HDC hTargetDC, int width, int height)
{
	HANDLE hFile;
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTemp;
	PBITMAPINFO pbi = CreateBitmapInfo(hBitmap);

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		return false; // could not allocate bitmap
	}

	GetDIBits(hTargetDC, hBitmap, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS);

	hFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false; // Could not open screenshot file
	}

	// type == BM
	hdr.bfType = 0x4d42;

	hdr.bfSize = (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

	// write the bitmap file header to file
	WriteFile(hFile, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);

	// write the bitmap header to file
	WriteFile(hFile, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);

	// copy the bitmap colour data into the file
	WriteFile(hFile, (LPSTR)lpBits, pbih->biSizeImage, &dwTemp, NULL);

	CloseHandle(hFile);

	GlobalFree((HGLOBAL)lpBits);
	return true;
}

bool SaveJPGFile(LPCTSTR filename, HBITMAP hBitmap)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* p_bmp = Bitmap::FromHBITMAP(hBitmap, NULL);
	CLSID npgclsid;
	GetEncoderClsid(L"image/jpeg", &npgclsid);
	p_bmp->Save(filename, &npgclsid, NULL);

	delete p_bmp;
	GdiplusShutdown(gdiplusToken);
	return true;
}

bool SavePNGFile(LPCTSTR filename, HBITMAP hBitmap)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* p_bmp = Bitmap::FromHBITMAP(hBitmap, NULL);
	CLSID npgclsid;
	GetEncoderClsid(L"image/png", &npgclsid);
	p_bmp->Save(filename, &npgclsid, NULL);

	delete p_bmp;
	GdiplusShutdown(gdiplusToken);
	return true;
}


void ExportToPictureFile(HWND hWnd, int type)
{
	HDC hDC = GetDC(hWnd);
	HDC hTargetDC = CreateCompatibleDC(hDC);
	RECT rect = { 0 };

	GetWindowRect(hWnd, &rect);

	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left,
		rect.bottom - rect.top);
	SelectObject(hTargetDC, hBitmap);
	PrintWindow(hWnd, hTargetDC, PW_CLIENTONLY);

	if(type == 0) // BITMAP
	SaveBMPFile(L"CaptureBMP.bmp", hBitmap, hTargetDC, rect.right - rect.left,
		rect.bottom - rect.top);
	else if (type == 1)
		SavePNGFile(L"CapturePNG.png", hBitmap);
	else if(type==2)
		SaveJPGFile(L"CaptureJPG.jpg", hBitmap);

	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hDC);
	DeleteDC(hTargetDC);
}


