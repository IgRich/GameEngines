#include "includer.h"
#include "MovementBoard.h"
#include "ActionsBoard.h"
#include "simpleLandscape.h"
#include "simpleFont.h"
#include <fstream>
#include <string>

#ifndef NPC_H
#define NPC_H

class NPC : public ActionsBoard{
private:
	virtual NPC& operator=(NPC& obj){
		//dummy
		return *this;
	}
	inline NPC(NPC&){
		//dummy
	}

	bool speak = false;
	bool hello = true;
	simpleFont font;
protected:

public:
	std::string* phraseArr;
	
	NPC();
	virtual ~NPC();
	bool boardWalk(simpleLandscape&);
	virtual int classType(){
		return 5;
	}
	bool descriptReader(std::fstream&);
	bool descriptReader(descriptor& desc);
	bool speakNPC();
	void turnOnSpeak();
};
#endif