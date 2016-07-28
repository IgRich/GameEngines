#include "includer.h"
#include "ActionsBoard.h"
#include "simpleLandscape.h"
#include "PersonState.h"
#include "Hero.h"
#include "StaticBoard.h"
#include <fstream>

#ifndef MOB_H
#define MOB_H

class Mob : public ActionsBoard,public PersonState{
private:
	virtual Mob& operator=(Mob& obj){
		//dummy
		return *this;
	}
	inline Mob(Mob&){
		//dummy
	}
protected:
	bool atack;				//atack flag
	float searchRadius;
	int pretAtackCount=0;
	bool prepareAtack;			//true - подготовка к атаке
	ActionsBoard prevState;
public:
	StaticBoard healBar;
	Mob();
	virtual ~Mob(){	}
	bool boardWalk(simpleLandscape&);
	virtual int classType(){
		return 6;
	}
	bool enemySearch(Board*);
	bool mobDeath(Board*, simpleLandscape&);
	bool descriptReader(const descriptor& desc);
	virtual bool setAction(const int& state, D3DXVECTOR3* arr, const float& rad,
		const int& numbPoint, Board* pObj, const float& speed);
	bool copy(Board*);
};
#endif