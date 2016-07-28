#include "includer.h"
#include "MovementBoard.h"

#ifndef ACTIONBOARD_H
#define ACTIONBOARD_H

class ActionsBoard: public MovementBoard{
protected:
	static const int MAX_STEP_SIZE = 10;
	D3DXVECTOR3* vectrArr;		//массив точек
	long int pointNumb;			//число точек(дл€ патрул€)
	long int curPoint;			//“екуща€ точка(куда идЄт при патрулирвоании)
	float radius;				//радиус брождени€
	float radiusAtack;			//что-бы не множить переменные

	enum ActionState{
		STAND		= 0,	//стоим
		GO			= 1,
		RAD_WALK	= 2,	//прогулка
		PATROL		= 3,	//патруль
		ATACK		= 4,	//атака
		RETREAT		= 5		//отступление
	} state;
private:
	virtual ActionsBoard& operator=(ActionsBoard& obj){	return *this;}
	ActionsBoard(ActionsBoard&){}
	//setFunc
	bool setPatrol(D3DXVECTOR3*, const int&);
	bool setStand(D3DXVECTOR3*);
	bool setRadWalk(D3DXVECTOR3*, const float&);
	bool setAtack(Board*, const float&);
	bool setRetreat(Board*);
	//useFunc
	ActionState usePatrol();
	ActionState useStand();
	ActionState useRadWalk();
	ActionState useAtack();
	ActionState useRetreat();
public:
	Board* target;				//дл€ атаки|отступлени€
	ActionsBoard();
	virtual ~ActionsBoard();
	virtual bool setAction(const int& state, D3DXVECTOR3* arr, const float& rad, \
						const int& numbPoint=1, Board* target=NULL, const float& speed=1);	//устанвока поведени€
	ActionState behavior();		//определени€ дальнейнего "поступка"
	virtual int classType(){
		return 4;
	}
	friend void copyState(ActionsBoard&, ActionsBoard&);
};

#endif