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
	AnimBoard::descriptReader(desc);	//��� ������ ����
	//��� healBar ����
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
			//�������� �� ������!
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
			//���������� �����.
	//����������� ������ ���, ��� ����� ����� ������ 4 �������(���� ���)
	if (prepareAtack == true){
		if (pretAtackCount == (clock() / CLOCKS_PER_SEC) % speedAtack){	//����������� ������� givDamage ������ �������
			pretAtackCount++;
			if (pretAtackCount == speedAtack){
				pretAtackCount = 0;
				prepareAtack = false;
			}
		}
	}
			//�������� �� ���� ����.� ������ ������:
	//�������� ��������� ����� � ���������� ������� ���������(��� �������� � ����)
	if (state!=ATACK && enemySearch(target) == true){
		copyState(prevState, *(dynamic_cast<ActionsBoard*>(this)));
		ActionsBoard::setAction(4, NULL, 0, 0, target, speedAtack);
	}
	ActionsBoard::ActionState choose = behavior();		//����� ���������� "����"
	switch (choose){
	case 0:
	case 1:
	case 2:
		MovementBoard::boardWalk(land);
		break;
	case 3:
		break;
	case 4:	//�����
		if (prepareAtack == false){
			AnimBoard::interrupt(animATACK, left);
			dynamic_cast<Hero*>(target)->damage(damageSize);
			prepareAtack = true;
		}
		AnimBoard::draw();
		break;
	}
		//���� �� ���� � ������ ������,�� �������� ����.���� player=(
	//�� ���������� ��� ���������� ���������(�� ��� ���������� ��!)
	if (state == ATACK && enemySearch(target) == false)
		copyState(*(dynamic_cast<ActionsBoard*>(this)), prevState);
		//���������� � ����� �� ����
	healBar.setWorldMat(worldMat._41, worldMat._42+160, worldMat._43);	//170 - ������, ���� ���������� �����
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
		return false;		//������� ������ ����
	goToDest(enemy.x, enemy.y, enemy.z);	//� ��� ��� ������. "��� �� ��!"(�)��������� ��������
	return true;
}
bool Mob::mobDeath(Board* pObj, simpleLandscape& land){
	unregInLand(land);			//������� ������� ��� � �����-�������
		//���������� �������
	D3DXMATRIX rotX, rotY;		
	D3DXMatrixRotationX(&rotX,PI/2);
	D3DXMatrixRotationY(&rotY,-PI/4);
	D3DXMatrixMultiply(&rotX, &rotX, &rotY);
	D3DXMatrixMultiply(&worldMat,&rotX,&worldMat);
		//������ ����������� ����
	//�������� ������� �������������� � ����
	//��������� ��� ��� ������ �� �����
	StaticBoard* deadBoard;
	deadBoard = new StaticBoard;
	deadBoard->copy(this);
	deadBoard->setWorldMat(worldMat);
	deadBoard->chooseTexture(10);
	land.addToStaticObjectList(deadBoard);
	return true;
}
//����� ���������� � ����� ActionsBoard, ���� ��� �� ������� ^_^
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
	healBar.bas=getPointer(dynamic_cast<Mob*>(obj)->healBar.bas);		//�������� ������
	return TRUE;
}