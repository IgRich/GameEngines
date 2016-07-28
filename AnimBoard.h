#include "includer.h"
#include "board.h"
#include <fstream>

#ifndef ANIMBOARD_H
#define ANIMBOARD_H

class AnimBoard: public Board{
private:
	int* animStage = NULL;		//номер текстур анимации animStage={0,5,10,14} =>	[0,5],[6,9],[10,13] - 3 анимации с такими кадрами
	int numbOfStage;			//сколько всего анимаций
	int curStage, prevStage;	//Какую анимацию рисовать
	long double tempTime;		//Переменные для отрисовки анимаций
	long int animFrame;

	bool animInterrupt;		//прерывание текущей анимации
	virtual AnimBoard& operator=(AnimBoard& obj){return *this;}
	inline AnimBoard(AnimBoard&){}
	bool setAnimStage(const int*, const int&);	//for desriptReader()
protected:
		
public:
	enum{
		animHOLD  =	0,
		animRUN   =	1,
		animATACK =	2,
		animDEATH =	3
	}possibleAnimState;

	AnimBoard();
	virtual ~AnimBoard();
	bool descriptReader(std::fstream&);			
	virtual bool descriptReader(const descriptor&);
	bool draw();								//отрисовка анимации с учетом stage
	bool setStage(const int&,const bool& left);		//установка анимации
	virtual int classType(){					//ID класса
		return 2;
	}
	bool interrupt(const int& numb, const bool& left);
	virtual bool copy(Board* obj);
};
#endif /*ANIMBOARD_H*/








