#define _CRT_SECURE_NO_WARNINGS
//#include "stdafx.h"
#include "Win32Project3.h"
#include "appWindow.h"
#include "InputSystem.h"
#include "Sprites.h"
#include "simpleFont.h"
#include "simpleLandscape.h"
//new ИЕРАРХИЯ
#include "staticBoard.h"
#include "AnimBoard.h"
#include "MovementBoard.h"
#include "Hero.h"
#include "ActionsBoard.h"
#include "NPC.h"
#include "Mob.h"
#include "staticBoard.h"
#include "ImageBasis.h"

using namespace std;

IDirect3D9       *g_pD3D=NULL;
IDirect3DDevice9 *g_pD3DDevice=NULL;
IDirect3DTexture9 *texture=NULL;
	/*for splash screen*/
ID3DXSprite *spriteS_Screen = NULL;
IDirect3DTexture9 *textureS_Screen = NULL;

D3DPRESENT_PARAMETERS d3dpp;
HWND        g_hWnd1;
HINSTANCE 	hInst1;

bool initSplashScreen(){
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "image/splash/splashScreen.tga", 1000, 750, D3DX_DEFAULT, 0, 
								D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
								D3DCOLOR_RGBA(0, 0, 0, 0), 0, NULL, &textureS_Screen);
	if (FAILED(D3DXCreateSprite(g_pD3DDevice, &spriteS_Screen)))
		return FALSE;
	return TRUE;
}
bool doSplashScreen(const int& alphaNumb){
	if (FAILED(g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255,255, 255, 255), 1.0f, 0)))
		Error("Debug error!");
	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		spriteS_Screen->Begin(D3DXSPRITE_ALPHABLEND);
		spriteS_Screen->Draw(textureS_Screen, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, alphaNumb));
		spriteS_Screen->End();
		g_pD3DDevice->EndScene();
	}
	else
		Error("BeginScene error!");
	if (g_pD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST){
		g_pD3DDevice->Reset(&d3dpp);
	}
	return TRUE;
}
		/*end of splash screen*/
bool DoInit();
void DoShutdown();
bool DoFrame();
bool prepareObject();

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow){
	WNDCLASSEX wcex;
	MSG        Msg;
	g_hInst = hInst;
	getWNDCLASSEX(wcex, hInst, "appForDirectX3D");
	if (!RegisterClassEx(&wcex))
		return FALSE;
	if (!(g_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_szClass, g_szCaption, WS_CAPTION | WS_SYSMENU, 0, 0, WNDWIDTH, WNDHEIGHT, NULL, NULL, hInst, NULL)))
		return FALSE;
	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);
	if (DoInit() == FALSE)
		return FALSE;

	ZeroMemory(&Msg, sizeof(MSG));	//заполняет память нулями
	ShowCursor(FALSE);

	if ((pDIDMouse = InitMouse(g_hWnd, g_pDI)) == NULL){
		Error("InitMouse failed!");
		return FALSE;
	}
	if((pDIDKeyboard = InitKeyboard(g_hWnd, g_pDI)) == NULL){
		Error("InitKeyboard failed!");
		return FALSE;
	}
	srand(time(NULL));	//for rand()
			//splashscreen
	/*initSplashScreen();
	for (int i = 0; i<=510; i++){
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			if (i <= 255)
				doSplashScreen(i);
			else
				doSplashScreen(510-i);
	}*/
	if (prepareObject() == FALSE)
		return FALSE;
			//main Cycle
	while (Msg.message != WM_QUIT) {
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
	}
		if (DoFrame() == FALSE)
			break;
	}
	DoShutdown();
	UnregisterClass(g_szClass, hInst);
	return Msg.wParam;
}

D3DXVECTOR3 cameraVect(0,360,-500);
D3DXVECTOR3 pEye(cameraVect),
			pAt(0.0f, 140.0f, 0.0f),
			pUp(0.0f, 1.0f, 0.0f);
D3DXMATRIX matProj, matView, matTrans, matWorld;
D3DXVECTOR3 scale(1.0f, 1.0f, 1.0f), dest(512.0f, 384.0f, 1.0f),
			boardOffset, dv(pEye-pAt);
RECT Rect = { 0, 0, 400, 400 };
char str[256];
bool DoInit(){
	D3DDISPLAYMODE	d3ddm;
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
		Error("Direct3DCreate9 error!");
		return FALSE;
	}
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		Error("GetAdapterDisplayMode error!");
		return FALSE;
	}
	windowedParametrs(d3dpp, d3ddm);
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))){
		Error("CreateDevice error!");
		return FALSE;
	}
	ImageBasis::gDevice = g_pD3DDevice;
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//свет - выкл
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);		//z-буфер - вкл
			//три функции для установки альфа-каналов
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//включение
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);					//цветовой ключ устанавливаем(что не рисуем)
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//функция обработки вершин

	D3DXMatrixOrthoLH(&matProj, 1024, 768, -2000, 5000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixLookAtLH(&matView, &(pAt+cameraVect), &pAt, &pUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	
	//**Инициализация ввода данных**//
	if (FAILED(DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, NULL))){
		Error("DirectInput8Create!");
		return FALSE;
	}
	return TRUE;
}

simpleFont smp;
Sprites hpBar;
simpleLandscape landscape;
		//курсор
ID3DXSprite *cursor = NULL;
IDirect3DTexture9 *cursTexture = NULL;
Hero player;
StaticBoard	*statObject = NULL;
StaticBoard	*copyObj = NULL;
Mob			*mobArrUnic = NULL;
Mob			*mobArrCopy = NULL;
NPC			*npcArrUnic = NULL;
NPC			*npcArrCopy = NULL;
StaticBoard* setStaticUnicObject(simpleLandscape& land, fstream& file){
	//Уникальные текстуры
	if (!file){
		Error("File error!");
		return false;
	}
	descriptor temp;
	int size;
	if (!(file >> size))
		return NULL;
	StaticBoard* staticObject;
	staticObject = new StaticBoard[size];
	for (int i = 0; i < size; i++){
		if (temp.setDescriptorStatic(file) == false)
			return NULL;
		staticObject[i].descriptReader(temp);
		staticObject[i].setWorldMat(temp.coordX, 0, temp.coordZ);
		land.addToStaticObjectList(&staticObject[i]);
	}
	return staticObject;
}
StaticBoard* setStaticCopyObject(StaticBoard* unicObject, simpleLandscape& land, fstream& file){
	//Уникальные текстуры
	if (!file){
		Error("File error!");
		return NULL;
	}
	int size,id,x,z;
	string str;
	if (!(file >> size))
		return NULL;
	StaticBoard* copyObject;
	copyObject = new StaticBoard[size];
	for (int i = 0; i < size; i++){
		file >> str;
		if (str[0] == '#'){
			str.clear();
			i--;
		}
		else{
			id = str.length();
			file.seekg(-id, ios::cur);
			if (!(file >> id >> x >> z))
				return NULL;
			copyObject[i].copy(&unicObject[id]);
			copyObject[i].setWorldMat(x, 0, z);
			landscape.addToStaticObjectList(&copyObject[i]);
		}
	}
	return copyObject;
}
Mob* setMobUnic(Hero *player, simpleLandscape&, fstream& file){
	Mob* array;
	int size;
	if (!file){
		Error("File error!");
		return false;
	}
	file >> size;
	if (size <= 0)
		return false;
	else
		array = new Mob[size];
	descriptor tmp;
	int X, Z, state,point,radius=1;
	D3DXVECTOR3* patrolVectr;
	for (int i = 0; i < size; i++){
		tmp.setDescriptorAnim(file);
		array[i].descriptReader(tmp);
		file >> X >> Z;
		array[i].setWorldMat(X,0,Z);
		file >> state;
		switch (state){
		case 0:
		case 1:
			patrolVectr = new D3DXVECTOR3[1];
			file >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
			array[i].setAction(state, patrolVectr, radius, 1, player, 3);
			delete[] patrolVectr;
			break;
		case 2:
			patrolVectr = new D3DXVECTOR3[1];
			file >> radius >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
			array[i].setAction(state, patrolVectr, radius, 1, player, 3);
			delete[] patrolVectr;
			break;

		case 3:
			file >> point;
			patrolVectr = new D3DXVECTOR3[point];
			for (int i = 0; i < point;i++)
				file >> patrolVectr[i].x >> patrolVectr[i].y >> patrolVectr[i].z;
			array[i].setAction(state, patrolVectr, radius, point, player, 3);
			delete[] patrolVectr;
			break;
		case 4:
			/*prototype atack*/
			break;
		case 5:
			/*prototype retreat*/
			break;
		}
		landscape.addToObjectList(&array[i]);
	}
	return array;
}
Mob* setMobCopy(Hero *player,Mob* unicMob, simpleLandscape&, fstream& file){
	Mob* array;
	int size;
	if (!file){
		Error("File error!");
		return false;
	}
	file >> size;
	if (size <= 0)
		return false;
	else
		array = new Mob[size];
	int id(0),X(0), Z(0), state(0), 
		point(0),radius(0);
	D3DXVECTOR3* patrolVectr;
	string str;
	for (int i = 0; i < size; i++){
		file >> str;
		if (str[0] == '#'){
			str.clear();
			i--;
		}
		else{
			id = str.length();
			file.seekg(-id, ios::cur);
			if (!(file >> id >> X >> Z >> state)){
				Error("setMobCopy error!");
				return false;
			}
			array[i].copy(&unicMob[id]);
			array[i].setWorldMat(X, 0, Z);
			switch (state){
			case 0:
			case 1:
				patrolVectr = new D3DXVECTOR3[1];
				file >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
				array[i].setAction(state, patrolVectr, radius, 1, player, 3);
				delete[] patrolVectr;
				break;
			case 2:
				patrolVectr = new D3DXVECTOR3[1];
				file >> radius >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
				array[i].setAction(state, patrolVectr, radius, 1, player, 3);
				delete[] patrolVectr;
				break;

			case 3:
				file >> point;
				patrolVectr = new D3DXVECTOR3[point];
				for (int i = 0; i < point; i++)
					file >> patrolVectr[i].x >> patrolVectr[i].y >> patrolVectr[i].z;
				array[i].setAction(state, patrolVectr, radius, point, player, 3);
				delete[] patrolVectr;
				break;
			case 4:
				/*prototype atack*/
				break;
			case 5:
				/*prototype retreat*/
				break;
			}
			landscape.addToObjectList(&array[i]);
		}
	}
	return array;
}
NPC* setNPCUnic(Hero *player, simpleLandscape&, fstream& file){
	NPC* array;
	int size;
	if (!file){
		Error("File error!");
		return false;
	}
	file >> size;
	if (size <= 0)
		return false;
	else
		array = new NPC[size];
	descriptor tmp;
	int X, Z, state, point,
		radius = 1, phraseNumb;
	D3DXVECTOR3* patrolVectr;
	for (int i = 0; i < size; i++){
		tmp.setDescriptorAnim(file);
		array[i].descriptReader(tmp);
		file >> X >> Z;
		array[i].setWorldMat(X, 0, Z);
		file >> state;
		switch (state){
		case 0:
		case 1:
			patrolVectr = new D3DXVECTOR3[1];
			file >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
			array[i].setAction(state, patrolVectr, radius, 1, player, 3);
			delete[] patrolVectr;
			break;
		case 2:
			patrolVectr = new D3DXVECTOR3[1];
			file >> radius >> patrolVectr[0].x >> patrolVectr[0].y >> patrolVectr[0].z;
			array[i].setAction(state, patrolVectr, radius, 1, player, 3);
			delete[] patrolVectr;
			break;

		case 3:
			file >> point;
			patrolVectr = new D3DXVECTOR3[point];
			for (int i = 0; i < point; i++)
				file >> patrolVectr[i].x >> patrolVectr[i].y >> patrolVectr[i].z;
			array[i].setAction(state, patrolVectr, radius, point, player, 3);
			delete[] patrolVectr;
			break;
		case 4:
			/*prototype atack*/
			break;
		case 5:
			/*prototype retreat*/
			break;
		}

		//фразы
		file >> phraseNumb;
		file.seekg(2,ios::cur);
		array[i].phraseArr = new string[phraseNumb];
		string temp;
		for (int j = 0; j < phraseNumb; j++){
			std::getline(file, temp);
			while (temp.size()>0){
				array[i].phraseArr[j] += temp + '\n';
				temp.clear();
				std::getline(file, temp);
			}
		}

		landscape.addToObjectList(&array[i]);
	}
	return array;
}
bool prepareObject(){
	//**Создание спрайта(курсор)**//
	D3DXCreateTextureFromFile(g_pD3DDevice, "image/cursor.tga", &cursTexture);
	if (FAILED(D3DXCreateSprite(g_pD3DDevice, &cursor))) {
		Error("Cursor create error!");
		return FALSE;
	}
	landscape.setLandscape("image/backgroud/testLevelMap1.jpg", 10000, 5000);
			//картонки
	smp.LoadFont("Arial", RECT{ 0, 0, 100, 100 }, g_pD3DDevice);
	

					//player
	fstream file("image/Description/creatures.txt");
	if (!file){
		Error("File error!");
		return false;
	}
	descriptor descPlayer;
	descPlayer.setDescriptorAnim(file);
	player.descriptReader(descPlayer);
	player.regInLand(landscape);
	player.setMovementSpeed(4);
	player.setWorldMat(800, 0, 600);
	hpBar.setTexture("image/interface/heal.tga", 369, 352, 11, 1);
					//MOB
	file.close();
	file.open("image/Description/unicCreatures.txt");
	mobArrUnic = setMobUnic(&player, landscape, file);
	file.close();
	file.open("image/Description/copyCreatures.txt");
	mobArrCopy = setMobCopy(&player, mobArrUnic, landscape, file);
	file.close();
				/*StaticObjectUnic*/
	file.open("image/Description/unicStaticObject.txt");
	statObject = setStaticUnicObject(landscape, file);
	if (statObject == NULL){
		Error("UnicStatic initialization error!");
		return false;
	}
	file.close();
				/*StaticObjectCopy*/
	file.open("image/Description/staticObjectDisplacement.txt");
	copyObj = setStaticCopyObject(statObject, landscape, file);
	if (copyObj == NULL){
		Error("copyStatic initialization error!");
		return false;
	}
	file.close();
	file.open("image/Description/npcUnic.txt");
	npcArrUnic = setNPCUnic(&player,landscape,file);
	file.close();
	return TRUE;
}
Board* tmp;
void getMousePosition(char* mousePosition, const D3DXVECTOR3& mouse){
	char tmp[20];
	char demmyCharT[3] = "  ";
	strcat(mousePosition,"mouse: \n");
	_itoa_s(mouse.x - WNDWIDTH / 2, tmp, 10);
	strcat(mousePosition, tmp);
	strcat(mousePosition, demmyCharT);
	
	_itoa_s(mouse.y - WNDHEIGHT / 2, tmp, 10);
	strcat(mousePosition, tmp);
	strcat(mousePosition, demmyCharT);
	
	_itoa_s(mouse.z, tmp, 10);
	strcat(mousePosition, tmp);
	strcat(mousePosition, demmyCharT);
	strcat(mousePosition, "\n");
}
void heroHitPointShow(char* stat, Hero& player){
	char tmp[20];
	char demmyCharT[3] = "  ";
	strcat(stat, "HP:  ");
	_itoa_s(player.showHP(), tmp, 10);
	strcat(stat, tmp);
	strcat(stat, "\n");
}
bool DoFrame(){
	boardOffset=handlingKeyboard();		//Клавиатура
	cursorPosition(dest);		//Мышь
	D3DXMatrixIdentity(&matWorld);
	if (FAILED(g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0)))
		Error("Debug error!");
		//for text information
	getMousePosition(str, dest);
	heroHitPointShow(str, player);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		if (KEYDOWN(KeyStateBuffer, DIK_SPACE))
			player.atack(landscape);
		player.handlingKeyboard(boardOffset);
		pAt = player.getPosition();
		pAt.y = 140;		//на уровне головы НПС
		pEye = pAt + cameraVect;
		D3DXMatrixLookAtLH(&matView, &pEye, &pAt, &pUp);
		landscape.draw();		//ground
		landscape.reloadAllObject();
			//hero & NPC
		
		for (int i = 0; i < landscape.objectList.size(); i++){
			if (!landscape.objectList[i].obj->boardWalk(landscape)){
				landscape.objectList[i] = landscape.objectList[landscape.objectList.size() - 1];
				//tmp = landscape.objectList[landscape.objectList.size() - 1].obj;
				landscape.objectList.pop_back();
				//delete tmp;
			}
		}
		for (int i = 0; i<landscape.staticObjectList.size(); i++)
			landscape.staticObjectList[i]->draw();
		smp.drawFont(str, Rect);	//text
			//gui
		if (10*player.percentHP()>0 && 10*player.percentHP()<1)
			hpBar.drawSpriteNumb(1, &scale, &D3DXVECTOR3(655, 0, 1));
		else
			hpBar.drawSpriteNumb((int)(10*player.percentHP()), &scale, &D3DXVECTOR3(655, 0, 1));
			//cursor
		cursor->Begin(D3DXSPRITE_ALPHABLEND);
		cursor->Draw(cursTexture, NULL, &scale, &dest, D3DCOLOR_RGBA(255, 255, 255, 255));
		cursor->End();
		g_pD3DDevice->EndScene();
	}
	else
		Error("BeginScene error!");
	if (g_pD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST){
		g_pD3DDevice->Reset(&d3dpp);
	}
	str[0]='\0';		//clear debugTextMenu
	return TRUE;
}



void clearComObj(IUnknown* pObj){
	if (pObj != NULL){
		pObj->Release();
		pObj = NULL;
	}
}

void DoShutdown(){	//выключение граф.системы
	clearComObj(g_pD3DDevice);
	clearComObj(g_pD3D);
	clearComObj(texture);
	clearComObj(cursor);
	clearComObj(cursTexture);
	clearComObj(g_pDI);
	clearComObj(pDIDKeyboard);
	clearComObj(pDIDMouse);
	clearComObj(spriteS_Screen);
	clearComObj(textureS_Screen);
}
