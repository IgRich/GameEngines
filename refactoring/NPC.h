#include "simpleBoard.h"

#ifndef NPC_H
#define NPC_H
class Hero;
class NPC :public simpleBoard{
protected:
	D3DXVECTOR3 dv, offset, dest, beacon;
	D3DXMATRIX worldMap;
	D3DXVECTOR3 begPath, endPath;		//для патрулирования
	bool move;
	int rad;
	int movementStage;
public:
	NPC();
	virtual ~NPC();
	inline bool goToDest(const float& X, const float& Y, const float& Z);
	bool speakPhrase();	//Прототип функции разговора персонажа(баннер с текстом!)
	virtual bool setAroundWalk(const float&, const float&, const float&, const float&);
	bool setPatrol(const D3DXVECTOR3&, const D3DXVECTOR3&);
	virtual bool randomWalking(Hero& player, simpleLandscape& land);
	bool patrolWalking();
	bool boardWalk();
	bool draw();
	D3DXVECTOR3 getPosition();
	int type(){
		return 1;
	}
};
#endif	/* NPC_H */