#include "includer.h"
#include "board.h"
#include <fstream>

#ifndef ANIMBOARD_H
#define ANIMBOARD_H

class AnimBoard: public Board{
private:
	int* animStage = NULL;		//����� ������� �������� animStage={0,5,10,14} =>	[0,5],[6,9],[10,13] - 3 �������� � ������ �������
	int numbOfStage;			//������� ����� ��������
	int curStage, prevStage;	//����� �������� ��������
	long double tempTime;		//���������� ��� ��������� ��������
	long int animFrame;

	bool animInterrupt;		//���������� ������� ��������
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
	bool draw();								//��������� �������� � ������ stage
	bool setStage(const int&,const bool& left);		//��������� ��������
	virtual int classType(){					//ID ������
		return 2;
	}
	bool interrupt(const int& numb, const bool& left);
	virtual bool copy(Board* obj);
};
#endif /*ANIMBOARD_H*/








