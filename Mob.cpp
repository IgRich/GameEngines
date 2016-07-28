#include "Mob.h"

Mob::Mob(){
	PersonState(300,3,30);
	searchRadius = 200;
	ActionsBoard::radiusAtack = 30;
	setMovementSpeed(2.0f);
	atack = false;
	prepareAtack = true;
	damageSize = 20;
}

bool Mob::descriptReader(const descriptor& desc){
	AnimBoard::descriptReader(desc);	//Для самого моба
	//Для healBar моба
	std::fstream file("image/people/objectBar/defaultForMob.txt");
	if (!file){
		Error("Mob() error!");
		throw "error";
	}
	descriptor descTemp;
	descTemp.setDescriptorStatic(file);
	healBar.descriptReader(descTemp);
	file.close();
	return true; 
}
bool Mob::boardWalk(simpleLandscape& land){
			//проверка на СМЕРТЬ!
	if (showHP() <= 0){
		//mobDeath(target, land);
		D3DXMATRIX rotX, rotY;
		D3DXMatrixRotationX(&rotX, PI / 2);
		D3DXMatrixRotationY(&rotY, -PI / 4);
		D3DXMatrixMultiply(&rotX, &rotX, &rotY);
		D3DXMatrixMultiply(&worldMat, &rotX, &worldMat);
		StaticBoard* deadBoard;
		deadBoard = new StaticBoard;
		deadBoard->copy(this);
		deadBoard->setWorldMat(worldMat);
		deadBoard->chooseTexture(10);
		land.addToStaticObjectList(deadBoard);
		return false;
	}
			//подготовка атаки.
	//Вычисляется каждый раз, тут стоит атака каждые 4 секунды(пока что)
	if (prepareAtack == true){
		if (pretAtackCount == (clock() / CLOCKS_PER_SEC) % speedAtack){	//увеличиваем счетчик givDamage каждую секунду
			pretAtackCount++;
			if (pretAtackCount == speedAtack){
				pretAtackCount = 0;
				prepareAtack = false;
			}
		}
	}
			//Проверка на агро моба.В случае истины:
	//Включаем атакующий режим и запоминаем прошлое состояние(для возврата в него)
	if (state!=ATACK && enemySearch(target) == true){
		copyState(prevState, *(dynamic_cast<ActionsBoard*>(this)));
		ActionsBoard::setAction(4, NULL, 0, 0, target, speedAtack);
	}
	ActionsBoard::ActionState choose = behavior();		//выбор следующего "шага"
	switch (choose){
	case 0:
	case 1:
	case 2:
		MovementBoard::boardWalk(land);
		break;
	case 3:
		break;
	case 4:	//атака
		if (prepareAtack == false){
			AnimBoard::interrupt(animATACK, left);
			dynamic_cast<Hero*>(target)->damage(damageSize);
			prepareAtack = true;
		}
		AnimBoard::draw();
		break;
	}
		//Если мы были в боевом режиме,но потеряли цель.Убег player=(
	//То возвращаем его предыдущее состояние(не зря запоминали же!)
	if (state == ATACK && enemySearch(target) == false)
		copyState(*(dynamic_cast<ActionsBoard*>(this)), prevState);
		//Вычисление и вывод ХП моба
	healBar.setWorldMat(worldMat._41, worldMat._42+160, worldMat._43);	//170 - высота, пока константно задал
	if (10 * percentHP() > 0 && 10 * percentHP() < 1)
		healBar.chooseTexture(1);
	if (percentHP() == 1)
		healBar.chooseTexture(9);
	else
		healBar.chooseTexture((int)(10 * percentHP()));
	healBar.draw();
	return true;
}
bool Mob::enemySearch(Board* pObj){
	D3DXVECTOR3 enemy = pObj->getPosition();
	if ((abs(worldMat._41 - enemy.x) + abs(worldMat._42 - enemy.y) + abs(worldMat._43 - enemy.z))>searchRadius)
		return false;		//слишком далеко цель
	goToDest(enemy.x, enemy.y, enemy.z);	//а тут уже близка. "Иду на вы!"(с)Святослав Игоревич
	return true;
}
bool Mob::mobDeath(Board* pObj, simpleLandscape& land){
	unregInLand(land);			//вначале удаляем его с земли-матушки
		//вычисление матрицы
	D3DXMATRIX rotX, rotY;		
	D3DXMatrixRotationX(&rotX,PI/2);
	D3DXMatrixRotationY(&rotY,-PI/4);
	D3DXMatrixMultiply(&rotX, &rotX, &rotY);
	D3DXMatrixMultiply(&worldMat,&rotX,&worldMat);
		//создаём статический труп
	//копируем текущие характеристики в труп
	//добавляем его для вывода на экран
	StaticBoard* deadBoard;
	deadBoard = new StaticBoard;
	deadBoard->copy(this);
	deadBoard->setWorldMat(worldMat);
	deadBoard->chooseTexture(10);
	land.addToStaticObjectList(deadBoard);
	return true;
}
//потом переписать в класс ActionsBoard, пока что не трогаем ^_^
bool Mob::setAction(const int& state, D3DXVECTOR3* arr, const float& rad,
					const int& numbPoint, Board* pObj, const float& speed){
	if (pObj == NULL || speed<=0)
		return false;
	copyState(prevState, *(dynamic_cast<ActionsBoard*>(this)));
	ActionsBoard::setAction(state, arr, rad, numbPoint, pObj, speed);
	return true;
}

bool Mob::copy(Board* obj){
	AnimBoard::copy(obj);
	healBar.bas=getPointer(dynamic_cast<Mob*>(obj)->healBar.bas);		//проблема решена
	return TRUE;
}