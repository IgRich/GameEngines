#ifndef STATICBOARD_H
#define STATICBOARD_H
#include "includer.h"
#include "Board.h"


class StaticBoard: public Board{
private:
	virtual StaticBoard& operator=(StaticBoard& obj){return *this;}
	StaticBoard(StaticBoard&){}
protected:
	int number;
	
public:
	StaticBoard();
	bool chooseTexture(const int&);
	bool draw();
	virtual int classType(){
		return 1;
	}
	bool addToNavMesh(simpleLandscape& land, const int& size);
	bool descriptReader(const descriptor&);
	virtual bool copy(Board* pObj){
		bas = getPointer(pObj->bas);
		width = pObj->width;
		zDepth = pObj->zDepth;
		return true;
	}
	
	
};
#endif /*STATICBOARD_H*/








