#ifndef MOB_CPP
#define MOB_CPP
#include "Mob.h"

bool Mob::enemySearch(Hero& player){
	//берем позицию игрока, если он достаточно близко к МОБу, то начинаем атаковать.
	D3DXVECTOR3 enemy = player.getPosition();
	if (abs(worldMap._41 - enemy.x) + abs(worldMap._43 - enemy.z) > 400)
		return false;
	goToDest(enemy.x, enemy.y, enemy.z);
	atack = true;
}
bool Mob::mobDeath(Hero& player, simpleLandscape& land){		//!!!!!!!!!!!
	this->unRegInLand(land);
	//Mob::~Mob();		//не забыть потом сделать деструкторы,а память утекает=(
	return true;
}
bool Mob::randomWalking(Hero& player, simpleLandscape& land){
	if (currentHP <= 0){
		mobDeath(player,land);
		return true;
	}
	//boardWalk();
	enemySearch(player);
	if (atack == true){		//если мы видим игрока,то идём атаковать
		if (move == true){
			//вначале определяем где он
			offset = dest - D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
			float dS = sqrt(offset.x*offset.x + offset.y*offset.y + offset.z*offset.z) / 6;
			dv.x = offset.x / dS;
			dv.y = offset.y / dS;
			dv.z = offset.z / dS;
			move = false;
		}
		if (abs(offset.x) + abs(offset.z)>75){
			//если мы слишком далеко,то нужно подойти
			bool step = landscape.checkNavMesh(D3DXVECTOR3(worldMap._41 + dv.x, worldMap._42 + dv.y, worldMap._43 + dv.z), this);
			if (step == true){	//если НавМеш позволяет сделать шаг, то шагаем
				worldMap._41 += dv.x;
				worldMap._42 += dv.y;
				worldMap._43 += dv.z;
				offset -= dv;
			}
			else{				//иначе сразу обнуляем смещение(что-бы дальше не пытаться шагать туда,куда не можем
				offset -= offset;
				dv -= dv;
			}
		}
		else{	//иначе мы уже достаточно близко,и будем атаковать врага
			offset.x = offset.y = offset.z = 0;
			if (givDamage == (clock() / CLOCKS_PER_SEC) % 4)	//увеличиваем счетчик givDamage каждую секунду
				givDamage++;
			if (givDamage == 4){	//как только доходим то 4-х, наносим урон и обнуляем счетчик для новой атаки.
				//нужно будет придумать как красивее написать, плохо константно прописывать скорость атаки
				player.damage(15);
				givDamage = 0;
			}
		}
		movementStage = 1;
		drawAnim(worldMap, movementStage);
		return true;
	}
	NPC::randomWalking(player,land);
	return true;

}
D3DXVECTOR3 Mob::getPosition(){
	return D3DXVECTOR3(worldMap._41, worldMap._42, worldMap._43);
}
Mob::Mob(){
	this->setMaxHP(200);
	this->setHP(200);
}
Mob::~Mob(){
	simpleBoard::~simpleBoard();
}
#endif /*MOB_CPP*/