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
	bool selectTexture(const int&,sVertex*);							//����� �������� (�����_��������, ������_��������)
	bool setTexture(const char*,const UINT&,const UINT&,const UINT&);	//�������� �������� (����, ������, ������_�����_��������, ����������_�������)
	bool setParam(const float&, const float&,const float&);				//������� �������� (X,Y,Z) 
	inline bool setAnimStage(const int*,const int&);					//����������� ������� ����_������ (������/����������_��������, �����_��������)
	bool drawAnim(const D3DXMATRIX&,const int&);						//������ ������� (���_��������, �����_��������)
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









