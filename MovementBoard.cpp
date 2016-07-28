#include "MovementBoard.h"

MovementBoard::MovementBoard(){
	dv = offset = dest = D3DXVECTOR3(0, 0, 0);
	move = false;
	left = true;
	movementSpeed = 1;
}
MovementBoard::MovementBoard(const float& speed){
	dv = offset = dest = D3DXVECTOR3(0, 0, 0);
	move = false;
	movementSpeed = speed;
}
bool MovementBoard::boardWalk(simpleLandscape& land){
	if (move == true){
		offset = dest - D3DXVECTOR3(worldMat._41, worldMat._42, worldMat._43);
		float dS = sqrt(offset.x*offset.x + offset.y*offset.y + offset.z*offset.z)/movementSpeed;
		dv.x = offset.x / dS;
		dv.y = offset.y / dS;
		dv.z = offset.z / dS;
		move = false;
	}
	if (abs(offset.x) + abs(offset.y) + abs(offset.z)>2){
		bool step = land.checkNavMesh(D3DXVECTOR3(worldMat._41 + dv.x, worldMat._42 + dv.y, worldMat._43 + dv.z), this);
		if (step == true){
			worldMat._41 += dv.x;
			worldMat._42 += dv.y;
			worldMat._43 += dv.z;
			offset -= dv;
			if (dv.x > 0)
				left = false;
			if(dv.x<0)
				left = true;
			AnimBoard::setStage(animRUN, left);
		}
		else{
			offset.x=offset.y=offset.z=0;
			dv.x = dv.y = dv.z = 0;
			AnimBoard::setStage(animHOLD, left);
		}
	}
	else
		AnimBoard::setStage(animHOLD, left);		//stand animation
	draw();
	return true;
}