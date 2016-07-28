//standart
#include <windows.h>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <typeinfo>
//DirectX 9
#include "d3d9.h"
#include "d3dx9.h"
#include "D3dx9core.h"
#include "DInput.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"DInput.lib")
#pragma comment(lib,"DInput8.lib")
#pragma comment(lib,"DXGuid.lib")

#ifndef INCLUDER_H
#define INCLUDER_H

#define _UNICODE
#define PI 3.14f
#define WNDWIDTH 1024
#define WNDHEIGHT 768
#define defaultSpeed(X) (0.5f*CLOCKS_PER_SEC*X)	//[пол секунды]*X - число тактов генератора с момента запуска программы
#define SPEED 5		//расстояние,которое объект проходит за единицу времени(один фрейм?)
#define VERTEXFVF (D3DFVF_XYZ| D3DFVF_TEX1)

typedef struct {
	FLOAT x, y, z;
	FLOAT u, v;
} sVertex;


void Error(const char* error);
#endif