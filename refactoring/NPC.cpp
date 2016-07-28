#include "NPC.h"

#ifndef NPC_CPP
#define NPC_CPP
bool NPC::draw(){
	drawAnim(worldMap, movementStage);
	return true;
}
NPC::NPC() :move(0), movementStage(0){
	//cur.x=cur.y=cur.z=0;
	D3DXMatrixIdentity(&worldMap);
}
NPC::~NPC(){
	simpleBoard::~simpleBoard();

}
bool NPC::setAroundWalk(const float& radius, const float& X, const float& Y, const float& Z){
	D3DXMatrixTranslation(&worldMap, X, Y, Z);
	beacon.x = X;
	beacon.y = Y;
	beacon.z = Z;
	rad = radius;
	movementStage = 1;
	return true;
}
bool NPC::setPatrol(const D3DXVECTOR3& beg, const D3DXVECTOR3& end){
	begPath = beg;
	endPath = end;
	D3DXMatrixTranslation(&worldMap, begPath.x, begPath.y, begPath.z);
	dest = endPath;
	move = true;
	return true;
}
bool NPC::patrolWalking(){
	if ((abs(offset.x) + abs(offset.z))<5){
		if ((abs(dest.x - begPath.x) + abs(dest.z - begPath.z))<5)
			dest = endPath;
		else
			dest = begPath;
		move = true;
	}
	boardWalk();

	return true;
}
bool NPC::randomWalking(Hero& player, simpleLandscape& land){
	//const Hero& player - чисто что бы одинаковый вызов был,как у Mob
	boardWalk();
	const int N = 5;
	if (rand() % 1000>N)		//каждые N секунд будет перемещаться
		return false;
	int chose = 0;
	chose = rand() % 4;
	switch (chose){
	case 0:
		goToDest(beacon.x + rad - rand() % (int)rad, 0, beacon.z + rad - rand() % (int)rad);
		break;
	case 1:
		goToDest(beacon.x + rad - rand() % (int)rad, 0, beacon.z - (rad - rand() % (int)rad));
		break;
	case 2:
		goToDest(beacon.x - (rad - rand() % (int)rad), 0, beacon.z - (rad - rand() % (int)rad));
		break;
	case 3:
		goToDest(beacon.x - (rad - rand() % (int)rad), 0, beacon.z + rad - rand() % (int)rad);
		break;
	}
	return true;
}
bool NPC::boardWalk(){
	if (move == true){
		offset = dest - D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
		float dS = sqrt(offset.x*offset.x + offset.y*offset.y + offset.z*offset.z) / 3;
		dv.x = offset.x / dS;
		dv.y = offset.y / dS;
		dv.z = offset.z / dS;
		move = false;
	}

	if (abs(offset.x) + abs(offset.z)>5){
		bool step = landscape.checkNavMesh(D3DXVECTOR3(worldMap._41 + dv.x, worldMap._42 + dv.y, worldMap._43 + dv.z), this);
		if (step == true){
			worldMap._41 += dv.x;
			worldMap._42 += dv.y;
			worldMap._43 += dv.z;
			offset -= dv;
		}
		else{
			offset -= offset;
			dv -= dv;
		}
	}
	movementStage = 1;
	drawAnim(worldMap, movementStage);
	return true;
}
inline bool NPC::goToDest(const float& X, const float& Y, const float& Z){
	dest.x = X;
	dest.y = Y;
	dest.z = Z;
	move = true;
	return true;
}
D3DXVECTOR3 NPC::getPosition(){
	return D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
}
#endif /*NPC_CPP*/