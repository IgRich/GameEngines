#ifndef MOB_H
#define MOB_H
#include "simpleboard.h"
#include "NPC.h"
#include "Hero.h"

class Hero;
class Mob : public NPC, public PersonState{
private:
	bool atack;
	int givDamage = 4;;
public:
	Mob();
	~Mob();
	bool enemySearch(Hero&);
	bool randomWalking(Hero&, simpleLandscape& land);
	D3DXVECTOR3 getPosition();
	bool mobDeath(Hero&,simpleLandscape&);
	int type(){
		return 2;
	}
};
#endif	/* MOOB_H */