#ifndef SIMPLEFONT_H
#define SIMPLEFONT_H
#include "includer.h"
#include <string>

class simpleFont{
private:
	ID3DXFont* pFont = NULL;;
	D3DXFONT_DESC descFont;
	RECT rect;
	ID3DXSprite *sprite= NULL;
	IDirect3DTexture9 *texture = NULL;
	int timer = 0;
public:
	simpleFont();
	~simpleFont();
	bool LoadFont(const char* fontName, const RECT& size,IDirect3DDevice9* pDevice);
	bool createSprite(IDirect3DDevice9* g_pD3DDevice);
	bool drawFont(const char* str);
	bool drawFont(const std::string& str);
	bool drawFont(const char* str, RECT& desteny);
	bool drawFontNPC(std::string& str, const D3DXVECTOR3& dest);
	//bool setRect(const RECT&);
};


#endif /* SIMPLEFONT_H */

