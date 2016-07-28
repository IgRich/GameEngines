#include "ImageBasis.h"

IDirect3DDevice9* ImageBasis::gDevice = NULL;

ImageBasis::ImageBasis(){
	texture = NULL;
	buffer = NULL;
	count = 0;
}
ImageBasis::~ImageBasis(){
	if (true)
		int x = 10;
	if (count>1){
		count--;
	}
	else{
		if (texture != NULL){
			texture->Release();
			texture = NULL;
		}
		if (buffer != NULL){
			buffer->Release();
			buffer = NULL;
		}
	}
}
IDirect3DTexture9* ImageBasis::getTexture(){
	return texture;
}
IDirect3DVertexBuffer9* ImageBasis::getBuf(){
	return buffer;
}
bool ImageBasis::setParam(const char* path, const int& boardX, const int& boardY,const int& textNumb,const int& column,
							const int& line, const int& animNumb, const int* animStage){
	if (boardX <= 0 || boardY <= 0)
		return FALSE;
	D3DXCreateTextureFromFile(gDevice, path, &texture);
	int vertexNumb = 0;
	float blockTextureSizeX = 1.0f/(float)column;
	float blockTextureSizeY = 1.0f/(float)line;
	animNumb == 0?vertexNumb=4*textNumb: vertexNumb=8*textNumb;
	sVertex* boardVerts = new sVertex[vertexNumb];
	for (int i = 0; i < vertexNumb;){
		boardVerts[i].x = -boardX/2;
		boardVerts[i].y = 0;
		boardVerts[i].z = 0;
		i++;
		boardVerts[i].x = -boardX/2;
		boardVerts[i].y = boardY;
		boardVerts[i].z = 0;
		i++;
		boardVerts[i].x = boardX/2;
		boardVerts[i].y = 0;
		boardVerts[i].z = 0;
		i++;
		boardVerts[i].x = boardX/2;
		boardVerts[i].y = boardY;
		boardVerts[i].z = 0;
		i++;
	}
	if (animNumb == 0){
		for (int i = 0, count = 0; count < textNumb; i += 4, count++)
			setTexture(&boardVerts[i], count, column, blockTextureSizeX, blockTextureSizeY);
	}
	else{
		int tempAnimNumb = 0;
		int i = 0;
		int k;
		while (tempAnimNumb < animNumb){		//animStage[] - реальное число анимаций(без зеркалировани€)
			int vertNumb = (animStage[tempAnimNumb + 1] - animStage[tempAnimNumb]);
			for (int temoNumbText = animStage[tempAnimNumb]; temoNumbText < animStage[tempAnimNumb + 1]; temoNumbText++){
				setTexture(&boardVerts[i], temoNumbText, column, blockTextureSizeX, blockTextureSizeY);
				i += 4;
			}
			for (int temoNumbText = animStage[tempAnimNumb]; temoNumbText < animStage[tempAnimNumb + 1]; temoNumbText++){
				setMirrorTexture(&boardVerts[i], temoNumbText, column, blockTextureSizeX, blockTextureSizeY);
				i += 4;
			}
			tempAnimNumb++;
		}
	}
	gDevice->CreateVertexBuffer(sizeof(sVertex)*vertexNumb, 0, VERTEXFVF, D3DPOOL_DEFAULT, &buffer, NULL);
	BYTE* ptr;
	buffer->Lock(0, 0, (void**)&ptr, 0);
	memcpy(ptr, boardVerts, sizeof(sVertex)*vertexNumb);
	buffer->Unlock();
	delete[] boardVerts;
	count++;
	return true;
}

void ImageBasis::setTexture(sVertex* arr, const int& text, const int& column, const float& sizeX, const float& sizeY){
	arr[0].u = (text%column)*sizeX;
	arr[0].v = (text/column+1)*sizeY;

	arr[1].u = (text%column)*sizeX;
	arr[1].v = (text/column)*sizeY;

	arr[2].u = (text%column +1)*sizeX;
	arr[2].v = (text/column +1)*sizeY;

	arr[3].u = (text%column +1)*sizeX;
	arr[3].v = (text/column)*sizeY;
}
void ImageBasis::setMirrorTexture(sVertex* arr, const int& text, const int& column, const float& sizeX, const float& sizeY){
	arr[0].u = (text%column +1)*sizeX;
	arr[0].v = (text/column +1)*sizeY;

	arr[1].u = (text%column +1)*sizeX;
	arr[1].v = (text/column)*sizeY;

	arr[2].u = (text%column)*sizeX;
	arr[2].v = (text/column +1)*sizeY;

	arr[3].u = (text%column)*sizeX;
	arr[3].v = (text/column)*sizeY;
}
ImageBasis* getPointer(ImageBasis* pObj){
	if (pObj->texture == NULL || pObj->buffer == NULL)
		return NULL;
	else{
		pObj->count++;
		return pObj;
	}
}