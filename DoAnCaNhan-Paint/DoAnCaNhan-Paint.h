#pragma once
#include "stdafx.h"
#include "resource.h"
#include "windows.h"
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "PaintLib.h"
using namespace PaintLibrary;
using namespace std;


#define LINE 0
#define RECTANGLE 1
#define SQUARE 2
#define ELLIPSE 3
#define CIRCLE 4
#define ARROW 5
#define STAR 6

void ExportToPictureFile(HWND hWnd, int type);
void Export(vector<CShape*> shapes, string filename);
void Import(string filename);
void Undo();
void Redo();
void OnCopy();

