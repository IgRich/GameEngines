#include "includer.h"
#include "MovementBoard.h"

#ifndef ACTIONBOARD_H
#define ACTIONBOARD_H

class ActionsBoard: public MovementBoard{
protected:
	static const int MAX_STEP_SIZE = 10;
	D3DXVECTOR3* vectrArr;		//������ �����
	long int pointNumb;			//����� �����(��� �������)
	long int curPoint;			//������� �����(���� ��� ��� ��������������)
	float radius;				//������ ���������
	float radiusAtack;			//���-�� �� ������� ����������

	enum ActionState{
		STAND		= 0,	//�����
		GO			= 1,
		RAD_WALK	= 2,	//��������
		PATROL		= 3,	//�������
		ATACK		= 4,	//�����
		RETREAT		= 5		//�����������
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
	Board* target;				//��� �����|�����������
	ActionsBoard();
	virtual ~ActionsBoard();
	virtual bool setAction(const int& state, D3DXVECTOR3* arr, const float& rad, \
						const int& numbPoint=1, Board* target=NULL, const float& speed=1);	//��������� ���������
	ActionState behavior();		//����������� ����������� "��������"
	virtual int classType(){
		return 4;
	}
	friend void copyState(ActionsBoard&, ActionsBoard&);
};

#endif