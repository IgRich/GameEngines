#include "PersonState.h"
#ifndef PERSONSTATE_CPP
#define	PERSONSTATE_CPP
PersonState::PersonState(){
	currentHP = maxHP = 300;
	speedAtack = 4;
	damageSize=60;
}
PersonState::PersonState(const int& HP,const int& speedAtack,const int& damage){
	currentHP = maxHP = HP;
	PersonState::speedAtack = speedAtack;
	damageSize=damage;
}
bool PersonState::damage(const int& dam){
	currentHP -= dam;
	if (currentHP <= 0)
		currentHP = 0;
	if (currentHP >= maxHP)
		currentHP = maxHP;
	return true;
}
bool PersonState::heal(const int& hel){
	currentHP += hel;
	if (currentHP >= maxHP)
		currentHP = maxHP;
	return true;
}
int PersonState::showHP(){
	return currentHP;
}
double PersonState::percentHP(){
	return (double)currentHP / (double)maxHP;
}
bool PersonState::setMaxHP(const int& numb){
	if (numb <= 0)		//нечего убивать так персонажей
		return false;
	maxHP = numb;
	currentHP = numb;
	return true;
}
bool PersonState::setHP(const int& numb){
	if (numb <= 0)		//нечего убивать так персонажей
		return false;
	if (numb >= maxHP)
		return false;
	currentHP = numb;
	return true;
}
#endif /*PERSONSTATE_CPP*/