#include "MovementBoard.h"
#include "PersonState.h"


#ifndef HERO_H
#define HERO_H
class Hero : public MovementBoard, public PersonState{
private:
	virtual Hero& operator=(Hero& obj){
		//dummy
		return *this;
	}
	inline Hero(Hero&){
		//dummy
	}
	
	int prepareAtackCount;
	float atackRadius;
	bool prepareAtack = false;
protected:
public:
	Hero();
	virtual ~Hero();
	bool handlingMouseClick(const D3DXVECTOR3&,simpleLandscape&);	//обработка клика мыши
	bool handlingKeyboard(const D3DXVECTOR3&);
	virtual int classType(){
		return -1;
	}
	bool boardWalk(simpleLandscape& land);
	bool atack(simpleLandscape& land);
};
#endif /* HERO_H*/