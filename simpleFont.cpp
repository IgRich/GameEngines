#include "simpleFont.h"

simpleFont::simpleFont(){
	ZeroMemory(&descFont, sizeof(D3DXFONT_DESC));
	if (pFont)
		pFont->Release();
	descFont.Height = -16;
	//descFont.Width = -12;
	descFont.Weight = 300;
	descFont.Italic = 0;
	descFont.CharSet = 0;
	descFont.OutputPrecision = 0;
	descFont.Quality = 0;
	descFont.PitchAndFamily = 0;
	descFont.MipLevels = D3DX_DEFAULT;
}
simpleFont::~simpleFont(){
	if (pFont = NULL){
		pFont->Release();
		pFont = NULL;
	}
	if (sprite = NULL){
		sprite->Release();
		sprite = NULL;
	}
	if (texture = NULL){
		texture->Release();
		texture = NULL;
	}
}
bool simpleFont::LoadFont(const char* fontName, const RECT& size, IDirect3DDevice9* pDevice){
	/*
	INT   Height;				высота ячейки символов(в отрицательных размерах)
	UINT  Width;				ширина ячейки символов(в отрицательных размерах)
	UINT  Weight;				"жирность" шрифта(0->1000)
	UINT  MipLevels;			ПОНЯТИЯ НЕ ИМЕЮ ЧТО ЭТО ТАКОЕ!
	BOOL  Italic;				Курсив (0 или 1)
	BYTE  CharSet;				ХЗ
	BYTE  OutputPrecision;		Точность отображения,согласно параметром выше...ХЗ
	BYTE  Quality;				выходное качество?ШТА
	BYTE  PitchAndFamily;		ХЗ
	TCHAR FaceName;				Название шрифта
	*/
	ZeroMemory(&descFont, sizeof(D3DXFONT_DESC));
	if (pFont)
		pFont->Release();
	descFont.Height = -16;
	descFont.Width = -8;
	descFont.Weight = 550;
	descFont.Italic = 0;
	descFont.CharSet = RUSSIAN_CHARSET;
	descFont.OutputPrecision = 0;
	descFont.Quality = 3;
	descFont.PitchAndFamily = 0;
	descFont.MipLevels = D3DX_DEFAULT;
	strcpy_s(descFont.FaceName, fontName);
	if (FAILED(D3DXCreateFontIndirect(pDevice, &descFont, &pFont))){
		Error("D3DXCreateFontIndirect error!");
		return false;
	}
	rect = size;
	return true;
}
bool simpleFont::createSprite(IDirect3DDevice9* g_pD3DDevice){
	D3DXCreateSprite(g_pD3DDevice, &sprite);
	D3DXCreateTextureFromFile(g_pD3DDevice, "image/cursor.tga", &texture);
	return true;
}
bool simpleFont::drawFont(const char* str, RECT& desteny){
	LPCTSTR tmpStr = str;
	pFont->DrawText(NULL, tmpStr, -1, &desteny, 0, D3DCOLOR_RGBA(255, 255, 255, 255));
	return true;
}
bool simpleFont::drawFontNPC(std::string& str, const D3DXVECTOR3& dest){
	//изучаем входную строку
	int column = 0, line = 1;
	int maxLenght = 0;
	for (int i = 0; i<str.size(); i++){
		if (str[i] == '\n'){
			line++;
			if (maxLenght < column)
				maxLenght = column;
			column = 0;
			continue;
		}
		column++;
	}
		//подбираем значение RECT
	//в зависимости от line и maxLenght
	rect.left = dest.x - 6 * maxLenght;
	rect.right = dest.x + 6 * maxLenght;
	rect.top	= dest.y-18*line;
	rect.bottom = dest.y;
	if (timer == (clock() / CLOCKS_PER_SEC) % 5){	//увеличиваем счетчик каждую единицу времени
		timer++;
		if (timer == 5){
			timer = 0;
			return false;
		}
	}
	LPCTSTR tmpStr = str.c_str();
	pFont->DrawText(NULL, tmpStr, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &rect, &D3DXVECTOR3(1, 1, 1), &D3DXVECTOR3(rect.left, rect.top, 1), D3DCOLOR_RGBA(0, 0, 0, 255));
	sprite->End();
	return true;
}