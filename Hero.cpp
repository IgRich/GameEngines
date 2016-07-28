#include "Hero.h"
#include "Mob.h"
#include "NPC.h"

Hero::Hero(){
	D3DXMatrixIdentity(&worldMat);
	PersonState(500,1,700);
	damageSize = 400;
	atackRadius=45;
	/*конструктор*/
}
Hero::~Hero(){
	/*prototype*/
}
bool Hero::boardWalk(simpleLandscape& land){
			/*подготовка атаки*/
	if (prepareAtack == true){
		if (prepareAtackCount == (clock()/CLOCKS_PER_SEC)%(speedAtack))	//увеличиваем счетчик givDamage каждую секунду
			prepareAtackCount++;
		if (prepareAtackCount == speedAtack){
			prepareAtackCount = 0;
			prepareAtack = false;
		}
	}
	MovementBoard::boardWalk(land);
	return true;
}

//мышь переделать!!!
bool Hero::handlingMouseClick(const D3DXVECTOR3& mouseClickVectr,simpleLandscape& land){
		D3DXVECTOR3 temp;
		temp = D3DXVECTOR3(worldMat._41, worldMat._42, worldMat._43) - mouseClickVectr;
		if (sqrt(temp.x*temp.x + temp.y*temp.y + temp.z *temp.z)>60){
			//если расстояние между векторами больше чем 60, то будет идти к противнику
			goToDest(mouseClickVectr);
			return true;
		}
		else{
			//обработка типов поверхности,куда мы нажали.
			Board* temp = land.whatIsThat(mouseClickVectr);
			if (temp == NULL)
				return true;
			switch (temp->classType()){
			case 0:		//simpleObject
				break;
			case 1:		//NPC
				break;
			case 6:		//Mob
				(dynamic_cast<Mob*>(temp))->damage(damageSize);
			}

		}
		return true;
	}
bool Hero::handlingKeyboard(const D3DXVECTOR3& inputMat){
	if ((abs(inputMat.x) + abs(inputMat.y) + abs(inputMat.z)) < SPEED)
		return true;
	if(inputMat.x>0)
		left = false;
	if (inputMat.x<0)
		left = true;
	goToDest(worldMat._41 + inputMat.x, worldMat._42 + inputMat.y, worldMat._43 + inputMat.z);
	return true;
}
bool Hero::atack(simpleLandscape& land){
		//Если к атаке готов
	if (prepareAtack == false){
		int vecSize=atackRadius/10;
		D3DXVECTOR3* atackVector;
		atackVector = new D3DXVECTOR3[vecSize];
		if (left == true){
			atackVector[0] = D3DXVECTOR3(worldMat._41 - 10, worldMat._42, worldMat._43);
			for (int i = 1; i < vecSize; i++){
				atackVector[i].x = atackVector[i-1].x - 10;
				atackVector[i].y = worldMat._42;
				atackVector[i].z = worldMat._43;
			}
		}
		else{
			atackVector[0] = D3DXVECTOR3(worldMat._41 + 10, worldMat._42, worldMat._43);
			for (int i = 1; i < vecSize; i++){
				atackVector[i].x = atackVector[i-1].x + 10;
				atackVector[i].y = worldMat._42;
				atackVector[i].z = worldMat._43;
			}
		}
		Board *pObj = NULL;
		int boardType;;
		for (int i = 0; i < vecSize; i++){
			pObj = land.whatIsThat(atackVector[i]);
			if (pObj != NULL && pObj != this){
				boardType = pObj->classType();
				switch (boardType){
					case 5:
						dynamic_cast<NPC*>(pObj)->turnOnSpeak();
						goto OutOfCycle;
					case 6:
						if (!interrupt(animATACK, left))
							goto OutOfCycle;
						else{
							dynamic_cast<Mob*>(pObj)->damage(damageSize);
							prepareAtack = true;
							goto OutOfCycle;
						}
				}
			}
		}
		OutOfCycle:
		delete[] atackVector;
	}
	return true;
}