#include "stdafx.h"
#include "osisp1.h"
#include "Shape.h"
#include "Line.h"
#include "Pencil.h"
#include "Rect.h"
#include "Circle.h"
#include <vector>
#include <commdlg.h>
#include <string.h>
#include "resource.h"
#include "Text.h"

#define MAX_LOADSTRING 100
using namespace std;

bool instrumentsStateArray[9];
std::vector<Shape*> figures;
HENHMETAFILE hEnhMtf;
int currPen;
bool isPrint = false;
HPEN pen;
POINT firstPoint;
bool isFirstClick;
HBRUSH brush;
Shape* tempFigure = NULL;
Shape* temp = NULL;
HINSTANCE hInst;					
TCHAR szTitle[MAX_LOADSTRING];			
TCHAR szWindowClass[MAX_LOADSTRING];	
bool isTouched = false;
static double scale = 1;
CHOOSECOLOR color;
COLORREF currColorPen;
COLORREF currColorBrush;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void clearInstrumentsStateArr();
HPEN createNewPen(COLORREF color, int penStyle);
bool createRequiredInstrument();
CHOOSECOLOR getColorDialog(HWND hwnd);
void SavePicture(HWND hWnd, HDC hdc, std::vector<Shape*> &figures, RECT rect);
void OpenPicture(HWND hWnd, HDC hdc);
HBRUSH createNewBrush(COLORREF color);
bool checkInstrumentsArray();


HPEN createNewPen(COLORREF color,int penStyle)
{
	HPEN pen = CreatePen(PS_SOLID, penStyle, color);
	return pen;
}

HBRUSH createNewBrush(COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	return brush;
}

CHOOSECOLOR getColorDialog(HWND hwnd)
{
	CHOOSECOLOR chooseColor = { 0 };
	COLORREF crCustColor[16];
	chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	chooseColor.hInstance = NULL;
	chooseColor.hwndOwner = hwnd;
	chooseColor.lpCustColors = crCustColor;
	chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	chooseColor.lCustData = 0L;
	chooseColor.lpfnHook = NULL;
	chooseColor.rgbResult = RGB(0x80, 0x80, 0x80);
	chooseColor.lpTemplateName = NULL;	
	return chooseColor;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;
	HACCEL hAccelTable;
	isFirstClick = true;
	clearInstrumentsStateArr();
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OSISP1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	currColorBrush = RGB(255, 255, 255);
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP1));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return (int) msg.wParam;
}

bool checkInstrumentsArray()
{
	for (int i = 0; i < 9; i++)
	{
		if (instrumentsStateArray[i])
			return true;
	}
	return false;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OSISP1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; 
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   color = getColorDialog(hWnd);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_LINE:
			clearInstrumentsStateArr();
			instrumentsStateArray[0] = true;
			break;
		case ID_RECTANGLE:
			clearInstrumentsStateArr();
			instrumentsStateArray[1] = true;
			break;
		case ID_ELLIPSE:
			clearInstrumentsStateArr();
			instrumentsStateArray[2] = true;
			break;
		case ID_POLIGONALLINE:
			clearInstrumentsStateArr();
			instrumentsStateArray[3] = true;
			break;
		case ID_PRINT:
			clearInstrumentsStateArr();
			instrumentsStateArray[1] = true;
			isPrint = true;
			break;
		case ID_FIGURES_POLYGON:
			clearInstrumentsStateArr();
			instrumentsStateArray[5] = true;
			break;
		case ID_FIGURES_TEXT:
			clearInstrumentsStateArr();
			instrumentsStateArray[4] = true;
			break;
		case ID_PENCIL:
			clearInstrumentsStateArr();
			instrumentsStateArray[8] = true;
			break;
		case ID_INSTRUMENTS_PAN:
			clearInstrumentsStateArr();
			instrumentsStateArray[6] = true;
			break;
		case ID_INSTRUMENTS_ZOOM:
			clearInstrumentsStateArr();
			instrumentsStateArray[7] = true;
			break;
		case ID_PEN_COLOR:
			if (ChooseColor(&color))
			{
				currColorPen = color.rgbResult;
			}
			break;
		case ID_COLOR_FILLCOLOR:
			if (ChooseColor(&color))
			{
				currColorBrush = color.rgbResult;
			}
			break;
		case ID_OPEN:
			hdc = GetDC(hWnd);
			OpenPicture(hWnd, hdc);
			break;
		case ID_SAVE:
			RECT rect;
			GetClientRect(hWnd, &rect);
			hdc = GetDC(hWnd);
			SavePicture(hWnd, hdc, figures, rect);
			break;
		case ID_LINETHICKNESS_THIN:
			currPen = 1;
			break;
		case ID_LINETHICKNESS_MEDIUM:
			currPen = 14;
			break;
		case ID_LINETHICKNESS_WIDE:
			currPen = 28;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		if (!instrumentsStateArray[3] && !instrumentsStateArray[5])
			isTouched = false;
		if (figures.size() != 0 && !instrumentsStateArray[6] && !instrumentsStateArray[7])
			figures.back()->setFinalPosition(LOWORD(lParam), HIWORD(lParam));
		//figures.back()->isDrawing = false;
		if (isPrint)
		{
			RECT tempRect;
			tempRect.left = figures.back()->startingPoint.y;
			tempRect.bottom = figures.back()->finalPoint.x;
			tempRect.right = figures.back()->finalPoint.y;
			tempRect.top = figures.back()->startingPoint.x;
			figures.pop_back();
			hdc = GetDC(hWnd);
			SavePicture(hWnd, hdc, figures, tempRect);
			isPrint = false;
			
		}
		if (instrumentsStateArray[8])
			isFirstClick = true;
		break;
	case WM_PAINT:
		XFORM xForm;
		xForm.eM11 = xForm.eM22 = scale;
		xForm.eM12 = xForm.eM21 = xForm.eDx = xForm.eDy = 0;
		
		hdc = BeginPaint(hWnd, &ps);
		SetGraphicsMode(hdc, GM_ADVANCED);
		SetWorldTransform(hdc, &xForm);
		for each (Shape* var in figures)
		{
			pen = createNewPen(var->penColor, var->thickness);
			brush = createNewBrush(var->brushColor);
			if (isPrint && var == figures.back())
			{
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
			}
			else
			{
				SelectObject(hdc, pen);
				SelectObject(hdc, brush);
			}
			
			var->draw(hdc);
			DeleteObject(pen);
			DeleteObject(brush);
		}
		EndPaint(hWnd, &ps);
		break;
	case  WM_CHAR:
		break;
	case WM_LBUTTONDOWN:
		if (checkInstrumentsArray()  )
		{
			if ((isFirstClick && instrumentsStateArray[5]) || (isFirstClick && instrumentsStateArray[6])){
				isFirstClick = false;
				firstPoint.x = LOWORD(lParam);
				firstPoint.y = HIWORD(lParam);
			}
			isTouched = true;
			if (!instrumentsStateArray[6] && !instrumentsStateArray[7]){
				createRequiredInstrument();
				figures.push_back(tempFigure);
				figures.back()->setStartPosition(LOWORD(lParam), HIWORD(lParam));
			}
			
		}
		
		break;
	case WM_MOUSEWHEEL:
		if (instrumentsStateArray[7]) {
			if ((GET_WHEEL_DELTA_WPARAM(wParam) > 0) && (scale < 2)) {
				scale += 0.1;
			}
			if ((GET_WHEEL_DELTA_WPARAM(wParam) < 0) && (scale > 1)) {
				scale -= 0.1;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_RBUTTONDOWN:
		isTouched = false;
		if (instrumentsStateArray[5]){
			HDC dc = GetDC(hWnd);
			tempFigure = new Line();
			tempFigure->thickness = currPen;
			tempFigure->penColor = currColorPen;
			figures.push_back(tempFigure);
			figures.back()->setStartPosition(LOWORD(lParam), HIWORD(lParam));
			figures.back()->setFinalPosition(firstPoint.x, firstPoint.y);
			figures.back()->isDrawing = false;
			isFirstClick = true;
		}
		RedrawWindow(hWnd, 0, NULL, RDW_INVALIDATE);
		break;
	case WM_MOUSEMOVE:
		if (isTouched && !instrumentsStateArray[6])
		{
			figures.back()->setFinalPosition(LOWORD(lParam), HIWORD(lParam));
		}
		if (isTouched && instrumentsStateArray[8])
		{
			if (figures.size() !=0)
			if (figures.back()->isDrawing)
			{
				figures.back()->setFinalPosition(LOWORD(lParam), HIWORD(lParam));
				figures.back()->isDrawing = false;
			}
			else
			{
				createRequiredInstrument();
				figures.push_back(tempFigure);
				figures.back()->setStartPosition(LOWORD(lParam), HIWORD(lParam));
			}

		}
		if (isTouched && instrumentsStateArray[6]){
			for each (Shape* var in figures)
			{
				var->pan(LOWORD(lParam) - firstPoint.x, HIWORD(lParam) - firstPoint.y);
			}
			firstPoint.x = LOWORD(lParam);
			firstPoint.y = HIWORD(lParam);
		}

		
		InvalidateRect(hWnd, NULL, TRUE);
		RedrawWindow(hWnd, 0, NULL, RDW_INVALIDATE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



void clearInstrumentsStateArr()
{
	for (int i = 0; i < 9; i++)
	{
		instrumentsStateArray[i] = false;
	}
}

string GetFilepathDialog(HWND hWnd, bool save) {
	static OPENFILENAME ofn;
	static	char fullpath[255], filename[256], dir[256];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = "Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
	if (save) {
		ofn.lpstrTitle = "Save picture as...";
		GetSaveFileName(&ofn);
	}
	else {
		ofn.lpstrTitle = "Open picture";
		GetOpenFileName(&ofn);
	}
	return fullpath;
}

void DrawAllFigures(HDC hdc, vector<Shape*> figures) {
	for each(Shape* figure in figures) {
		(*figure).draw(hdc);
	}
}

void SavePicture(HWND hWnd, HDC hdc, vector<Shape*> &figures, RECT rect) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 1);
	if (!fullpath.empty())
	{
		HDC mtfHdc;
		fullpath += ".emf";
		mtfHdc = CreateEnhMetaFile(hdc, fullpath.c_str(), NULL, NULL);
		PlayEnhMetaFile(mtfHdc, hEnhMtf, NULL);
		DrawAllFigures(mtfHdc, figures);
		hEnhMtf = CloseEnhMetaFile(mtfHdc);
		DeleteEnhMetaFile(hEnhMtf);
	}
}

void OpenPicture(HWND hWnd, HDC hdc) {
	static string fullpath;
	fullpath = GetFilepathDialog(hWnd, 0);
	if (!fullpath.empty())
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		hEnhMtf = GetEnhMetaFile(fullpath.c_str());
		PlayEnhMetaFile(hdc, hEnhMtf, &rect);
	}
}

bool createRequiredInstrument()
{
	int tempI = -1;
	for (int i = 0; i < 9; i++)
	{
		if (instrumentsStateArray[i])
		{
			tempI = i;
			break;
		}
	}
	switch (tempI)
	{
	case 0:
		tempFigure = new Line();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		break;
	case 1:
		tempFigure = new Rect();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		tempFigure->brushColor = currColorBrush;
		
		
		break;
	case 2:
		tempFigure = new Circle();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		tempFigure->brushColor = currColorBrush;
		break;
	case 3:
		tempFigure = new Line();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		tempFigure->brushColor = currColorBrush;
		break;
	case 4:
		tempFigure = new Text();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		tempFigure->brushColor = currColorBrush;
		break;
	case 5:
		tempFigure = new Line();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		tempFigure->brushColor = currColorBrush;
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		tempFigure = new Line();
		tempFigure->thickness = currPen;
		tempFigure->penColor = currColorPen;
		break;
	default:
		break;
	}
	return false;
}

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
