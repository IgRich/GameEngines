#ifndef STATICBOARD_CPP
#define STATICBOARD_CPP
#include "includer.h"
#include "StaticBoard.h"

class simpleLandscape;

StaticBoard::StaticBoard(){
	number = 0;
	width = 0;
	zDepth = 0;
}
bool StaticBoard::chooseTexture(const int& input){
	if (input < 0)
		return false;
	number = input;
	return true;
}
bool StaticBoard::draw(){
	D3DXMATRIX matWorldSaved;
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &matWorldSaved);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMat);
	g_pD3DDevice->SetTexture(0, bas->getTexture());
	g_pD3DDevice->SetStreamSource(0, bas->getBuf(), 0, sizeof(sVertex));
	g_pD3DDevice->SetFVF(VERTEXFVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*number, 2); //рисуем одну картонку
	g_pD3DDevice->SetTexture(0, NULL);								 //очищаем текстуру
	g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(sVertex));	 //очищаем поток вершин
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldSaved);			 //вернуть исходную не забываем!
	return true;
}
bool StaticBoard::descriptReader(const descriptor& file){
	Board::descriptReader(file);

	return true;
}
bool StaticBoard::addToNavMesh(simpleLandscape& land, const int& size){
	for (int j = 0; j < zDepth; j += 10){
		for (int i = 0; i <= size; i += 10){
			land.stateNavMeshOnVector(D3DXVECTOR3(worldMat._41 + i, worldMat._42, worldMat._43 + j), this);
			land.stateNavMeshOnVector(D3DXVECTOR3(worldMat._41 - i, worldMat._42, worldMat._43 + j), this);
		}
	}
	return true;
}
#endif /*STATICBOARD_CPP*/








