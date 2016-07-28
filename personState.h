#ifndef PERSONSTATE_H
#define	PERSONSTATE_H

class PersonState{
protected:
	long int maxHP;
	long int currentHP;

	int speedAtack;
	int damageSize;
public:
	PersonState();
	PersonState(const int& HP, const int& speedAtack, const int& damage);
	
	bool damage(const int& dam);
	bool heal(const int& hel);
	int showHP();
	bool setMaxHP(const int& numb);
	bool setHP(const int& numb);
	double percentHP();
};

#endif /*PERSONSTATE_H*/