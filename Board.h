#ifndef BOARD_H
#define BOARD_H
#include "includer.h"
#include "simpleLandscape.h"
#include "ImageBasis.h"

struct descriptor{
	//var
	char filePlace[256];
	int numb, column, line;
	float boardWidth, boardHeight;
	float coordX, coordZ, zDepth;
	int animNumb;
	int* array=NULL;

	//func:
	~descriptor(){
		if (array != NULL){
			delete[] array;
			array = NULL;
		}

	}
	bool descriptor::setDescriptorStatic(std::fstream& file){
		file >> filePlace;
		if (filePlace[0] == '#'){
			filePlace[0] = '\0';
			file >> filePlace;
		}
		file >> boardWidth >> boardHeight >> zDepth>>
			numb >> column >> line >> coordX >> coordZ;
		return true;
	}
	bool descriptor::setDescriptorAnim(std::fstream& file){
		file >> filePlace >> boardWidth >> boardHeight >> zDepth >>
				numb >> column >> line >> animNumb;
			array = new int[animNumb + 1];
			for (int i = 0; i<animNumb + 1; i++)
				file >> array[i];
			return true;
	}
};

class simpleLandscape;

class Board{
private:
	virtual Board& operator=(Board& obj){ return *this; }
	Board(Board&){}
protected:
	D3DXMATRIX worldMat;
public:
	ImageBasis* bas;
	int width, zDepth;
	virtual ~Board();
	Board();
	virtual bool descriptReader(std::fstream&);
	virtual bool descriptReader(const descriptor&);
	virtual bool draw() = 0;
	virtual int classType(){	return 0;	}
	virtual bool boardWalk(simpleLandscape&){
		return true;
	}
	bool regInLand(simpleLandscape&);
	bool unregInLand(simpleLandscape&);
	bool setWorldMat(const float&, const float&, const float&);
	bool setWorldMat(const D3DXMATRIX&);
	D3DXVECTOR3 getPosition(){
		return D3DXVECTOR3(worldMat._41, worldMat._42, worldMat._43);
	}
	virtual bool copy(Board*){	return false;	}
};
#endif /*BOARD_H*/








