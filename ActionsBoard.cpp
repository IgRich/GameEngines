#include "ActionsBoard.h"
const int forRadWalk = 5;		//пусть пока будет вот так,константно

bool ActionsBoard::setPatrol(D3DXVECTOR3* arr, const int& numb){
	if (arr == NULL || numb <= 0)
		return false;
	pointNumb = numb;
	vectrArr = new D3DXVECTOR3[pointNumb];
	for (int i = 0; i < pointNumb; i++)
		vectrArr[i] = arr[i];
	state = PATROL;
	return true;
}
bool ActionsBoard::setStand(D3DXVECTOR3* arr){
	if (arr == NULL)
		return false;
	vectrArr = new D3DXVECTOR3;
	*vectrArr = *arr;
	goToDest(*vectrArr);
	state = STAND;
	return true;
}
bool ActionsBoard::setRadWalk(D3DXVECTOR3* arr, const float& rad){
	if (arr == NULL || rad<=0)
		return false;
	radius = rad;
	vectrArr = new D3DXVECTOR3;
	*vectrArr = *arr;
	goToDest(*vectrArr);
	state = RAD_WALK;
	return true;
}
bool ActionsBoard::setAtack(Board* pObj, const float& speed){
	target = pObj;
	state = ATACK;
	return true;
}
bool ActionsBoard::setRetreat(Board* pObj){
	/*prototype*/
	state = RETREAT;
	return true;
}

ActionsBoard::ActionState ActionsBoard::usePatrol(){
	if ((abs(offset.x)+abs(offset.y)+abs(offset.z))<MAX_STEP_SIZE){
		if (curPoint >= (pointNumb-1))
			curPoint = 0;
		else
			curPoint++;
		goToDest(vectrArr[curPoint]);
	}
	return state;
}
ActionsBoard::ActionState ActionsBoard::useStand(){
	if ((abs(worldMat._41 - vectrArr->x) + abs(worldMat._42 - vectrArr->y)
		+ abs(worldMat._43 - vectrArr->z)) < MAX_STEP_SIZE)
		return state;
	return GO;
}
ActionsBoard::ActionState ActionsBoard::useRadWalk(){
	if (rand()%1000>forRadWalk)
		return STAND;
	int chose = rand() % 4;	//выбор направления движения
	switch (chose){
		case 0:
			goToDest(vectrArr[0].x+radius-rand()%(int)radius, 0 ,vectrArr[0].z+radius-rand()%(int)radius);
			break;
		case 1:
			goToDest(vectrArr[0].x+radius-rand()%(int)radius, 0 ,vectrArr[0].z-radius+rand()%(int)radius);
			break;
		case 2:
			goToDest(vectrArr[0].x-radius+rand()%(int)radius, 0 ,vectrArr[0].z+radius-rand()%(int)radius);
			break;
		case 3:
			goToDest(vectrArr[0].x-radius+rand()%(int)radius, 0 ,vectrArr[0].z-radius+rand()%(int)radius);
			break;
	}
	return GO;
}
ActionsBoard::ActionState ActionsBoard::useAtack(){
	//прототип
	D3DXVECTOR3 enemy = target->getPosition();
	if ((abs(worldMat._41 - enemy.x) + abs(worldMat._42 - enemy.y) + abs(worldMat._43 - enemy.z)) > radiusAtack){
		goToDest(enemy);
		return GO;
	}
	else
		return ATACK;
}
ActionsBoard::ActionState ActionsBoard::useRetreat(){
	//прототип
	return state;
}

ActionsBoard::ActionsBoard(){
	pointNumb = 1;
	curPoint = 0;
	radius = 0;
	vectrArr = new D3DXVECTOR3;
	vectrArr->x = vectrArr->y = vectrArr->z = 0;
	target = NULL;
	state = STAND;
	radiusAtack = 40;
}
ActionsBoard::~ActionsBoard(){
	if (vectrArr != NULL){
		delete[] vectrArr;
		vectrArr = NULL;
	}
	target = NULL;

}
bool ActionsBoard::setAction(const int& state, D3DXVECTOR3* arr, const float& rad, \
								const int& numbPoint, Board* pObj, const float& speed){
	if (vectrArr != NULL){
		delete[] vectrArr;
		vectrArr = NULL;
	}
	target = pObj;
	switch (state){
		case 0:
			setStand(arr);
			break;
		case 2:
			setRadWalk(arr, rad);
			break;
		case 3:
			setPatrol(arr, numbPoint);
			break;
		case 4:
			setAtack(pObj, speed);
			break;
		case 5:
			setRetreat(pObj);
			break;
	}
	return true;
}
ActionsBoard::ActionState ActionsBoard::behavior(){
	ActionsBoard::ActionState temp;
	switch (state){
		case STAND:
			temp = useStand();
			break;
		case RAD_WALK:
			temp = useRadWalk();
			break;
		case 3:
			temp = usePatrol();
			break;
		case 4:
			temp = useAtack();
			break;
		case 5:
			temp = useRetreat();
			break;
	}
	return temp;
}

void copyState(ActionsBoard& dest, ActionsBoard& source){
	dest.pointNumb = source.pointNumb;
	dest.vectrArr = new D3DXVECTOR3[dest.pointNumb];
	for (int i = 0; i < dest.pointNumb;i++)
		dest.vectrArr[i] = source.vectrArr[i];
	dest.curPoint = source.curPoint;
	dest.radius = source.radius;
	dest.target = source.target;
	dest.radiusAtack = source.radiusAtack;
	dest.state = source.state;
}
