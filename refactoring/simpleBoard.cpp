#ifndef SIMPLEBOARD_CPP
#define SIMPLEBOARD_CPP
#include "simpleboard.h"

bool simpleBoard::descriptReader(std::fstream& input){
	char filePlace[256];
	float width, height;
	int numb, column, line;
	float X, Y, Z;
	int animNumb;
	int beg, end;
	input >> filePlace >> width >> height
		>> numb >> column >> line
		>> X >> Y >> Z
		>> animNumb;
	int* array = new int[animNumb];
	for (int i = 0; i<animNumb; i++)
		input >> array[i];
	D3DXCreateTextureFromFile(g_pD3DDevice, filePlace, &texture);
	simpleBoard::textNumb = numb;
	simpleBoard::column = column;
	simpleBoard::line = line;
	blockTextureSizeX = 1.0f / (float)column;
	blockTextureSizeY = 1.0f / (float)line;
	if (!setParam(X, Y, Z))
		Error("param error!");
	if (!setAnimStage(array, animNumb))
		Error("stage error!");
	delete[] array;
	return true;
}
bool simpleBoard::selectTexture(const int& numb, sVertex* bVerts){		//пока что все в линию и константно
	bVerts[0].u = numb*blockTextureSizeX;
	bVerts[0].v = (numb / column + 1)*blockTextureSizeY;

	bVerts[1].u = numb*blockTextureSizeX;
	bVerts[1].v = (numb / column)*blockTextureSizeY;

	bVerts[2].u = (numb + 1)*blockTextureSizeX;
	bVerts[2].v = (numb / column + 1)*blockTextureSizeY;

	bVerts[3].u = (numb + 1)*blockTextureSizeX;
	bVerts[3].v = (numb / column)*blockTextureSizeY;
	return true;
}
bool simpleBoard::setTexture(const char* filePlace, const UINT& width, const UINT& textSize, const UINT& numb){
	D3DXCreateTextureFromFile(g_pD3DDevice, filePlace, &texture);
	//D3DXCreateTextureFromFileEx(g_pD3DDevice,filePlace,width,328,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,0,NULL,&texture);
	textNumb = numb;
	column = width / textSize;
	line = numb / column;
	blockTextureSizeX = 1.0f / (float)column;
	blockTextureSizeY = 1.0f / (float)line;
	return true;
}
bool simpleBoard::setParam(const float& X, const float& Y, const float& Z){
	if (texture == NULL)
		return false;
	if (X <= 0 || Y <= 0 || Z <= 0)
		return false;
	sVertex* boardVerts = new sVertex[(4 * textNumb)];
	for (int i = 0; i<(4 * textNumb);){
		boardVerts[i].x = X/2;
		boardVerts[i].y = 0;
		boardVerts[i].z = -Z / 2;
		i++;
		boardVerts[i].x = X / 2;
		boardVerts[i].y = Y;
		boardVerts[i].z = -Z / 2;
		i++;
		boardVerts[i].x = -X / 2;
		boardVerts[i].y = 0;
		boardVerts[i].z = Z / 2;
		i++;
		boardVerts[i].x = -X / 2;
		boardVerts[i].y = Y;
		boardVerts[i].z = Z / 2;
		i++;
	}
	int count = 0;
	for (int i = 0; i<(4 * textNumb); i += 4){
		selectTexture(count, &boardVerts[i]);
		count++;
	}
	g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*(4 * textNumb), 0, VERTEXFVF, D3DPOOL_DEFAULT, &boardBuf, NULL);
	BYTE* ptr;
	boardBuf->Lock(0, 0, (void**)&ptr, 0);
	memcpy(ptr, boardVerts, sizeof(sVertex)*(4 * textNumb));
	boardBuf->Unlock();
	tempTime = 0;
	delete[] boardVerts;
	return true;
}
bool simpleBoard::setAnimStage(const int* array, const int& animNumb){
	animStage = new int[animNumb];
	for (int i = 0; i<animNumb; i++){
		animStage[i] = array[i];
	}
	return true;
}
bool simpleBoard::drawAnim(const D3DXMATRIX& worldMap, const int& stage){
	//сохранение изначальной матрицы мира
	D3DXMATRIX matWorldSaved;
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &matWorldSaved);
	//анимаци€ объекта
	if (tempTime <= 0){									//обнуление. аждый раз при прекращении отрисовки(деструктор) обнул€ем эту переменную
		tempTime = clock() / defaultSpeed(0.25);			//¬ зависимости от коэффициента врем€ идЄт. ≈сли 0,5 - то каждую половину секунды.
		animFrame = animStage[stage];
	}
	if ((clock() / defaultSpeed(0.25)) - tempTime >= 1){
		tempTime = clock() / defaultSpeed(0.25);
		if (animFrame<animStage[stage + 1] - 1)
			animFrame++;
		else
			animFrame = animStage[stage];
	}	//конец алгоритма анимации

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMap);
	g_pD3DDevice->SetTexture(0, texture);
	g_pD3DDevice->SetStreamSource(0, boardBuf, 0, sizeof(sVertex));
	g_pD3DDevice->SetFVF(VERTEXFVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * animFrame, 2);//рисуем одну картонку
	g_pD3DDevice->SetTexture(0, NULL);								 //очищаем текстуру
	g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(sVertex));	 //очищаем поток вершин
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldSaved);			 //вернуть исходную не забываем!

	return true;
}
simpleBoard::~simpleBoard(){
	//допилить unregInLan(), удаление указател€ из objList
	if (texture != NULL){
		texture->Release();
		texture = NULL;
	}
	if (boardBuf != NULL){
		boardBuf->Release();
		boardBuf = NULL;
	}
}
bool simpleBoard::regInLand(simpleLandscape& land){
	land.addToObjectList(this);
	return true;
}
bool simpleBoard::unRegInLand(simpleLandscape& land){
	if (land.delToObjectList(this))
		return true;
	else
		return false;
}
#endif //SIMPLEBOARD_CPP


