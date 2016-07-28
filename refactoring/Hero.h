#include "simpleBoard.h"
#include "personState.h"
#include "Mob.h"

#ifndef HERO_H
#define HERO_H

class Hero : public simpleBoard, public PersonState{
protected:
	D3DXVECTOR3 dv, offset, dest;
	D3DXMATRIX worldMap;		//������� ���������
	bool move;					//�������� ��� ���
	int movementStage;			//����� ��������
public:
	Hero();
	~Hero();
	//bool heroMovement(const D3DXVECTOR3& pAt, const bool& attack);
	bool draw();
	inline bool goToMouseClick(const D3DXVECTOR3& mouse);
	D3DXVECTOR3 boardWalk();
	bool atackMob();
	D3DXVECTOR3 getPosition();
	bool handlingMouseClick(const D3DXVECTOR3&);	//��������� ����� ����
	int type();
};
#endif /* HERO_H*/
