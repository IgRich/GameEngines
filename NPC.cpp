#include "NPC.h"

NPC::NPC(){
	/*прототип*/
}
NPC::~NPC(){
	if (phraseArr != NULL){
		delete[] phraseArr;
		phraseArr = NULL;
	}
}
bool NPC::descriptReader(std::fstream& file){
	AnimBoard::descriptReader(file);
	font.LoadFont("Arial cir", RECT{ 0, 0, 100, 100 }, ImageBasis::gDevice);
	return true;
}

bool NPC::speakNPC(){
	D3DXMATRIX view, proj, world;
	D3DXVECTOR3 p1;
	D3DVIEWPORT9 pViewport;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &world);
	g_pD3DDevice->GetViewport(&pViewport);
	D3DXVec3Project(&p1, &D3DXVECTOR3(worldMat._41, worldMat._42 + 170, worldMat._43), &pViewport, &proj, &view, &world);
	if (hello == true){
		if (!font.drawFontNPC(phraseArr[0], p1)){
			hello = false;
			return false;
		}
	}
	else{
		if (!font.drawFontNPC(phraseArr[1], p1))
			return false;
	}
	return true;
}
bool NPC::descriptReader(descriptor& desc){
	AnimBoard::descriptReader(desc);
	font.LoadFont("Arial", RECT{ 0, 0, 100, 100 }, ImageBasis::gDevice);
	font.createSprite(ImageBasis::gDevice);
	return true;
}
void NPC::turnOnSpeak(){
	speak = true;
}
bool NPC::boardWalk(simpleLandscape& land){
		//ФОНТ!
	if (speak == true){
		if (!speakNPC())
			speak = false;
	}
	behavior();
	MovementBoard::boardWalk(land);
	//погнали с отрисовкой ФОНТА!
	return true;
}