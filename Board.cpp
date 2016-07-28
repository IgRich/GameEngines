#include "board.h"

bool Board::descriptReader(std::fstream& input){
	char filePlace[256];
	int numb, column, line;
	float boardWidth, boardHeight;
	input >> filePlace >> boardWidth >> boardHeight
		>> numb >> column >> line;
	D3DXMatrixIdentity(&worldMat);
	bas = new ImageBasis;
	bas->setParam(filePlace, boardWidth, boardHeight, numb, column, line);
	return true;
}
bool Board::descriptReader(const descriptor& file){
	D3DXMatrixIdentity(&worldMat);
	width = file.boardWidth;
	zDepth = file.zDepth;
	worldMat._41 = file.coordX;
	worldMat._42 = 0;
	worldMat._43 = file.coordZ;
	bas = new ImageBasis;
	bas->setParam(file.filePlace, file.boardWidth, file.boardHeight, file.numb, file.column, file.line);
	return true;
}
Board::~Board(){
	if (bas != NULL){
		delete bas;
		bas = NULL;
	}
}
bool Board::setWorldMat(const float& X, const float& Y, const float& Z){
	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixTranslation(&worldMat, X, Y, Z);
	return true;
}
bool Board::setWorldMat(const D3DXMATRIX& mat){
	worldMat = mat;
	return true;
}
bool Board::regInLand(simpleLandscape& land){
	land.addToObjectList(this);
	return true;
}
bool Board::unregInLand(simpleLandscape& land){
	if (land.delToObjectList(this))
		return true;
	else
		return false;
}
Board::Board(){
	D3DXMatrixIdentity(&worldMat);
}
