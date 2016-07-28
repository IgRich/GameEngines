#include "Hero.h"

#ifndef HERO_CPP
#define HERO_CPP

Hero::Hero(){
	setHP(500);
	dest.x = dest.y = dest.z = 0.0f;
	move = 0;
	movementStage = 0;
	D3DXMatrixIdentity(&worldMap);
}
D3DXVECTOR3 Hero::boardWalk(){
	if (move == true){	//флаг нужен чисто что-бы выполнять эту часть функции
		offset = dest - D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);				//вектор перемещения
		float dS = sqrt(offset.x*offset.x + offset.y*offset.y + offset.z*offset.z) / SPEED;	//время нужное для прохождение(число фреймов?)
		dv = offset / dS;		//вектор смещения за фрейм
		move = false;			//поэтому обнуляем его
		dest.x = dest.y = dest.z = 0;
	}
	if ((abs(offset.x) + abs(offset.z)) > 3){
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
	else
		dv.x = dv.y = dv.z = 0;
	movementStage = 1;
	return D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
}
/*bool Hero::heroMovement(const D3DXVECTOR3& pAt, const bool& attack){
	if ((abs(worldMap._41 - pAt.x) + abs(worldMap._42 - pAt.y) + abs(worldMap._43 - pAt.z))<1)
		movementStage = 0;
	else{
		movementStage = 1;
		worldMap._41 = pAt.x;
		worldMap._42 = pAt.y;
		worldMap._43 = pAt.z;
	}
	if (attack == true)
		movementStage = 2;
	drawAnim(worldMap, movementStage);
	return true;
}*/
inline bool Hero::goToMouseClick(const D3DXVECTOR3& mouse){	//dest(x,y,z) - координаты назначения
	dest.x = mouse.x;
	dest.y = mouse.y;
	dest.z = mouse.z;
	move = true;		//говорит о том что будет движение
	return true;
}
D3DXVECTOR3 Hero::getPosition(){
	return D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
}
bool Hero::handlingMouseClick(const D3DXVECTOR3& clickVector){
	D3DXVECTOR3 temp;
	temp = D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43) - clickVector;
	if (sqrt(temp.x*temp.x + temp.y*temp.y + temp.z *temp.z)>60){
		//если расстояние между векторами больше чем 50, то будет идти к противнику
		goToMouseClick(clickVector);
		return true;
	}
	else{
		simpleBoard* temp = landscape.whatIsThat(clickVector);
		void* temp1;
		if (temp == NULL)
			return true;
		switch (temp->type()){
			case 0:		//simpleObject
				break;
			case 1:		//NPC
				break;
			case 2:		//Mob
				(dynamic_cast<Mob*>(temp))->damage(20);
		}

	}
	return true;
}
int Hero::type(){
	return 4;
}
bool Hero::atackMob(){
	//атака моба! in progress...
	return true;
}
bool Hero::draw(){
	simpleBoard::drawAnim(worldMap, movementStage);
	return true;
}
Hero::~Hero(){
	simpleBoard::~simpleBoard();
}
#endif