#include "includer.h"

#ifndef IMAGEBASIS_H
#define IMAGEBASIS_H

class ImageBasis{
private:
	IDirect3DTexture9 *texture;			
	IDirect3DVertexBuffer9* buffer;
	long int count;		//число указателей на этот объект

	void setTexture(sVertex* arr, const int& text, const int& column, const float& sizeX, const float& sizeY);
	void setMirrorTexture(sVertex* arr, const int& text, const int& column, const float& sizeX, const float& sizeY);
				/*что бы другие не могли использовать их*/
	ImageBasis(ImageBasis&){}
	ImageBasis& operator=(ImageBasis&){}
public:
	static IDirect3DDevice9 *gDevice;
	ImageBasis();
	~ImageBasis();
	IDirect3DTexture9* getTexture();
	IDirect3DVertexBuffer9* getBuf();
	bool setParam(const char* path, const int& boardX, const int& boardY, const int& textNumb, const int& line,
		const int& column, const int& animNumb = 0, const int* animStage = NULL);
	friend ImageBasis* getPointer(ImageBasis* pObj);
};
	/*обязательно прописать это после определения девайса*/
//ImageBasis::gDevice = /*pointer_to_device*/;
#endif