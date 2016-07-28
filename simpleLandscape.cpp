#include "stdafx.h"
#include "Board.h"
#include "StaticBoard.h"
#define vectorEqual(A,B,E) ((abs(A.x-B.x)+abs(A.y-B.y)+abs(A.z-B.z))<E)

simpleLandscape::~simpleLandscape(){\
	if (texture != NULL){
		texture->Release();
		texture = NULL;
	}
	if (landBuf != NULL){
		landBuf->Release();
		landBuf = NULL;
	}
	if (navMesh != NULL){
		for (int i = 0; i < navY; i++)
			delete[] navMesh[i];
		navMesh = NULL;
	}
	objectList.clear();
	staticObjectList.clear();

}
bool simpleLandscape::draw(){
	g_pD3DDevice->SetTexture(0, texture);
	g_pD3DDevice->SetStreamSource(0, landBuf, 0, sizeof(sVertex));
	g_pD3DDevice->SetFVF(VERTEXFVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(sVertex));
	return 0;
}
bool simpleLandscape::setLandscape(char* filePlace, const float& X, const float& Y){
	if (X <= 0 || Y <= 0)
		return false;
	BYTE *Ptr;
	D3DXCreateTextureFromFile(g_pD3DDevice, filePlace, &texture);
	sVertex land[4] = {
		{ X, 0, Y, 0.0f, 1.0f },
		{ X, 0, 0, 0.0f, 0.0f },
		{ 0, 0, Y, 1.0f, 1.0f },
		{ 0, 0, 0, 1.0f, 0.0f }
	};
	navX = X / navMeshStep;
	navY = Y / navMeshStep;
	navMesh = new Board**[navY];
	for (int i = 0; i < navY; i++){
		navMesh[i] = new Board*[navX];
		for (int j = 0; j < navX; j++)
			navMesh[i][j] = NULL;
	}
	g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*4, 0, VERTEXFVF, D3DPOOL_DEFAULT, &landBuf, NULL);
	landBuf->Lock(0, 0, (void**)&Ptr, 0);
	memcpy(Ptr, land, sizeof(sVertex)*4);
	landBuf->Unlock();
	return true;
}
bool simpleLandscape::checkNavMesh(D3DXVECTOR3& vectr, Board* pObj){
	int y = vectr.z / navMeshStep;	//сверху->вниз
	int x = vectr.x / navMeshStep;	//справа->налево
	if (x >= navX || x < 0 || y >= navY || y < 0)
		return false;
	if (navMesh[y][x] == NULL || navMesh[y][x] == pObj)
		return true;
	else
		return false;
}
bool simpleLandscape::clearNavMeshOnVector(D3DXVECTOR3& oldVec, Board* pObj){
	int y = oldVec.z / navMeshStep;
	int x = oldVec.x / navMeshStep;
	for (int i = -1; i<2; i++){
		for (int j = -1; j < 2; j++){
			if (x + j >= navX || x + j < 0 || y + i >= navY || y + i < 0)
				continue;
			if (navMesh[y + i][x + j] == NULL || navMesh[y + i][x + j] != pObj)
				continue;
			else
				navMesh[y + i][x + j] = NULL;;
		}
	}
	return true;
}
bool simpleLandscape::stateNavMeshOnVector(D3DXVECTOR3& oldVec, Board* pObj){
	int y = oldVec.z / navMeshStep;
	int x = oldVec.x / navMeshStep;
	for (int i = -1; i<2; i++){
		for (int j = -1; j < 2; j++){
			if (x + j >= navX || x + j < 0 || y + i >= navY || y + i < 0)
				continue;
			if (navMesh[y + i][x + j] == NULL)
				navMesh[y + i][x + j] = pObj;
		}
	}
	return true;
}
bool simpleLandscape::addToObjectList(Board* pObj){
	objPosition tmp;
	tmp.obj = pObj;
	tmp.oldPos = pObj->getPosition();
	objectList.push_back(tmp);
	return true;
}
bool simpleLandscape::delToObjectList(Board* deleteObj){
	for (int i = 0; i < objectList.size(); i++){
		/*	В идеале очищаем нав-меш, потом создаем новый статический объект
		на основе моба(трупик). После удаляем его с "Земли", и деструктор.
		*/
		if (objectList[i].obj == deleteObj){
			clearNavMeshOnVector(objectList[i].oldPos, objectList[i].obj);
			objectList[i] = objectList[objectList.size() - 1];
			objectList.pop_back();
			return true;
		}
	}
	return false;
}
bool simpleLandscape::reloadAllObject(){
	for (int i = 0; i < objectList.size(); i++){
		if (vectorEqual(objectList[i].oldPos, objectList[i].obj->getPosition(), 5))
			continue;
		this->clearNavMeshOnVector(objectList[i].oldPos, objectList[i].obj);
		objectList[i].oldPos = objectList[i].obj->getPosition();
		this->stateNavMeshOnVector(objectList[i].oldPos, objectList[i].obj);
	}
	return true;
}
Board* simpleLandscape::whatIsThat(const D3DXVECTOR3& clickVector){
	int x = clickVector.x / navMeshStep;
	int y = clickVector.z / navMeshStep;
	if (x >= navX || x < 0 || y >= navY || y < 0)
		return NULL;
	else
		return navMesh[y][x];
}
bool simpleLandscape::addToStaticObjectList(Board* pStatObject){
	staticObjectList.push_back(dynamic_cast<StaticBoard*>(pStatObject));
	dynamic_cast<StaticBoard*>(pStatObject)->addToNavMesh(*this, dynamic_cast<StaticBoard*>(pStatObject)->width/2);
	return true;
}
bool simpleLandscape::delToStaticObjectList(Board* pStatObject){
	//прототип
	return true;
}

