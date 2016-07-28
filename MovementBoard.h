#include "includer.h"
#include "AnimBoard.h"
#include "simpleLandscape.h"

#ifndef MOVEMENTBOARD_H
#define MOVEMENTBOARD_H

class simpleLandscape;

class MovementBoard: public AnimBoard{
private:
	virtual MovementBoard& operator=(MovementBoard& obj){return *this;}
	inline MovementBoard(MovementBoard&){}
protected:
	D3DXVECTOR3 dv, offset, dest;		//	шаг/смещение/путь
	bool move;				//	флаг движения
	float movementSpeed;
	bool left = true;
public:
	MovementBoard();
	MovementBoard(const float& speed);
	virtual ~MovementBoard(){}
	inline bool goToDest(const float& X, const float& Y, const float& Z){
		dest.x = X;
		dest.y = Y;
		dest.z = Z;
		move = true;
		return true;
	}
	inline bool goToDest(const D3DXVECTOR3& in){
		dest = in;
		move = true;
		return true;
	}
	virtual bool boardWalk(simpleLandscape&);
	int classType(){
		return 3;
	}
	bool setMovementSpeed(const float& tmp){
		if (tmp <= 0)
			return false;
		movementSpeed = tmp;
		return true;
	}
};


#endif
