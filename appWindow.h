#include "includer.h"

#ifndef APPWINDOW_H
#define APPWINDOW_H


HWND          g_hWnd;
HINSTANCE     g_hInst;
const char	g_szClass[]   = "appForDirectX3D";
const char	g_szCaption[] = "MyFirstGame";

long FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam){
    switch(uMsg) {
		case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool getWNDCLASSEX(WNDCLASSEX& wcex,HINSTANCE& hInst,const char* className){
	wcex.cbSize        = sizeof(wcex);
    wcex.style         = CS_CLASSDC;
    wcex.lpfnWndProc   = WindowProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInst;
    wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = className;
    wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	return true;
}

void windowedParametrs(D3DPRESENT_PARAMETERS& param,D3DDISPLAYMODE& mode){
	ZeroMemory(&param, sizeof(param));
	param.Windowed = TRUE;
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.BackBufferFormat = mode.Format;
	param.EnableAutoDepthStencil = TRUE;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	param.BackBufferWidth = WNDWIDTH;
	param.BackBufferHeight = WNDHEIGHT;
}

#endif /* APPWINDOW_H */

