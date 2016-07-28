#ifndef SIMPLELANDSCAPE_H
#define SIMPLELANDSCAPE_H
#include "includer.h"
#include "PersonState.h"
#include "Board.h"


extern IDirect3DDevice9 *g_pD3DDevice;
class Board;

struct objPosition{
	Board* obj;
	D3DXVECTOR3 oldPos;
};
const int navMeshStep = 10;

class simpleLandscape{
	
	IDirect3DTexture9 *texture = NULL;
	IDirect3DVertexBuffer9* landBuf = NULL;
	float width, height;
	Board*** navMesh;
	long int navX, navY;
public:
	std::vector<objPosition> objectList;
	std::vector<Board*> staticObjectList;
	virtual ~simpleLandscape();
	bool setLandscape(char*, const float& X, const float& Y);
	bool draw();
	bool checkNavMesh(D3DXVECTOR3&, Board*);
	bool clearNavMeshOnVector(D3DXVECTOR3&, Board*);
	bool stateNavMeshOnVector(D3DXVECTOR3&, Board*);
	bool addToObjectList(Board*);
	bool delToObjectList(Board*);
	bool addToStaticObjectList(Board*);
	bool delToStaticObjectList(Board*);
	bool reloadAllObject();
	Board* whatIsThat(const D3DXVECTOR3& clickVector);
};
#endif /* SIMPLELANDSCAPE_H */