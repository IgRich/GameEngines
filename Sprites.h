#include "includer.h"

#ifndef SPRITES_H
#define SPRITES_H

extern IDirect3DDevice9 *g_pD3DDevice;
extern IDirect3D9       *g_pD3D;

class Sprites{
private:
	IDirect3DTexture9*	texture=NULL;
	ID3DXSprite*		sprite12 = NULL;
	RECT rect;
	long int numberOfTexture;
	long int column;			//[0,column-1]
	long int line;				//[0,line-1]
	long int textSizeX, textSizeY;
	long int animFrame;
	long int animFrameTemp;
	long double tempTime;
public:
	~Sprites();
	bool setTexture(char* filePlace, const UINT& width, const UINT& height, const int& textNumb, const int& columnNumb);
	bool selectTexture(const int& numb);
	bool drawSpriteNumb(const int& input, D3DXVECTOR3* scale, D3DXVECTOR3* dest);
	bool animatedDraw(const D3DXVECTOR3& scale, const D3DXVECTOR3& dest);
	
};


#endif /* SPRITES_H */