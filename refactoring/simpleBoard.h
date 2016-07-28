#ifndef SIMPLEBOARD_H
#define SIMPLEBOARD_H
#include "includer.h"
#include "simpleLandscape.h"
class Hero;
class simpleBoard{
protected:
	IDirect3DTexture9 *texture= NULL;
	IDirect3DVertexBuffer9* boardBuf=NULL;
	int column,textNumb,line;
	float blockTextureSizeX,blockTextureSizeY;
	int* animStage = NULL;
	long double tempTime;	//debug
	long int animFrame;
	
public:
	virtual ~simpleBoard();
	bool descriptReader(std::fstream&);
	bool selectTexture(const int&,sVertex*);							//выбор текстуры (номер_текстуры, начало_картонки)
	bool setTexture(const char*,const UINT&,const UINT&,const UINT&);	//загрузка картинки (путь, ширина, размер_одной_текстуры, количество_текстур)
	bool setParam(const float&, const float&,const float&);				//размеры картонки (X,Y,Z) 
	inline bool setAnimStage(const int*,const int&);					//определения позиции аним_кадров (массив/дескриптор_анимаций, число_анимаций)
	bool drawAnim(const D3DXMATRIX&,const int&);						//Рендер картона (где_рисовать, какую_анимацию)
	bool regInLand(simpleLandscape& land);
	bool unRegInLand(simpleLandscape& land);
	virtual D3DXVECTOR3 getPosition(){
		return D3DXVECTOR3(0, 0, 0);
	}
	virtual bool patrolWalking(){
		return true;
	}
	virtual bool randomWalking(Hero& player, simpleLandscape& land){
		return true;
	}
	virtual bool setPatrol(const D3DXVECTOR3&, const D3DXVECTOR3&){
		return true;
	}
	virtual bool setAroundWalk(const float&, const float&, const float&, const float&){
		return true;
	}
	virtual int type() = 0;

};
#endif //#infdef SIMPLEBOARD_H









