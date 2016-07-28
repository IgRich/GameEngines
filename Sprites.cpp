#ifndef SPRITES_CPP
#define SPRITES_CPP
#include "Sprites.h"

Sprites::~Sprites(){
	if (texture!=NULL){
		texture->Release();
		texture = NULL;
	}
	if (sprite12 != NULL){
		sprite12->Release();
		sprite12 = NULL;
	}
}
bool Sprites::setTexture(char* filePlace, const UINT& width, const UINT& height, const int& textNumb, const int& columnNumb){
	LPCTSTR temp = (LPCTSTR)filePlace;
	if (FAILED(D3DXCreateTextureFromFileEx(g_pD3DDevice, temp, width, height, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, NULL, &texture))){
		Error("Sprites::setTexture error!");
	}
	numberOfTexture = textNumb;
	column = columnNumb;
	line = textNumb / columnNumb;
	textSizeX = width / column;
	textSizeY = height / line;
	tempTime = 0;	//debug
	D3DXCreateSprite(g_pD3DDevice, &sprite12);
	return true;
}
bool Sprites::selectTexture(const int& numb){
	if (numb >= numberOfTexture || numb < 0)
		Error("Sprites::SelectTexture error");
	rect.left = textSizeX*(numb%column);
	rect.right = textSizeX*(numb%column + 1);
	rect.top = textSizeY*(numb / column);
	rect.bottom = textSizeY*(numb / column + 1);
	return true;
}
bool Sprites::drawSpriteNumb(const int& input, D3DXVECTOR3* scale, D3DXVECTOR3* dest){
	selectTexture(input);
	sprite12->Begin(D3DXSPRITE_ALPHABLEND);
	sprite12->Draw(texture, &rect, scale, dest, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite12->End();
	return true;
}
bool Sprites::animatedDraw(const D3DXVECTOR3& scale, const D3DXVECTOR3& dest){
	if (tempTime <= 0){		//обнуление.Каждый раз при прекращении отрисовки(деструктор) обнуляем эту переменную
		tempTime = clock() / defaultSpeed(0.25);	//0.25 - четверть от пол секунды 
		animFrame = 0;
	}
	if ((clock() / defaultSpeed(0.25)) - tempTime >= 1){
		tempTime = clock() / defaultSpeed(0.25);
		if (animFrame >= (numberOfTexture - 1))
			animFrame = 0;
		else
			animFrame++;
	}
	selectTexture(animFrame);
	sprite12->Begin(D3DXSPRITE_ALPHABLEND);
	sprite12->Draw(texture, &rect, &scale, &dest, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite12->End();
	return true;
}

#endif