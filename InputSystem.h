#include "includer.h"


#ifndef DINPUTSYSTEM_H
#define DINPUTSYSTEM_H
#define KEYDOWN(name,key) (name[key]&0x80)
#define MouseButtonState(MouseState,x) ((MouseState.rgbButtons[x] & 0x80)? FALSE : TRUE)

//подсистема ввода
extern IDirect3DDevice9	*g_pD3DDevice;
IDirectInput8	*g_pDI;
IDirectInputDevice8* InitKeyboard(HWND, IDirectInput8*);
IDirectInputDevice8* InitMouse(HWND,IDirectInput8*);
IDirectInputDevice8* pDIDKeyboard;
IDirectInputDevice8* pDIDMouse;
char KeyStateBuffer[256];				// Буфер данных для хранения состояний клавиш
long 	g_MouseXPos = WNDWIDTH/2, 		// Координаты мыши
		g_MouseYPos = WNDHEIGHT/2, 
		g_MouseZPos = 0;		
DIMOUSESTATE MouseState;				// Буфер данных для хранения состояния мыши

IDirectInputDevice8* InitMouse(HWND hWnd, IDirectInput8* pDI){
	IDirectInputDevice8 *pDIDevice;
	if(FAILED(pDI->CreateDevice(GUID_SysMouse, &pDIDevice, NULL)))
		return NULL;
	if(FAILED(pDIDevice->SetDataFormat(&c_dfDIMouse))) {
		pDIDevice->Release();
		return NULL;
	}
	if(FAILED(pDIDevice->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
		/*	Флаги кооперации
		DISCL_BACKGROUND		//
		DISCL_EXCLUSIVE			//полный захват устройства мыши окном
		DISCL_FOREGROUND		
		DISCL_NONEXCLUSIVE		//Позволяет другим так-же использовать мышь(выйти за пределы ока и использовать в другом окне)
		DISCL_NOWINKEY			//Отключить клавишу с логотипом Windows. 
		*/
		pDIDevice->Release();
		return NULL;
	}
	/*if(FAILED(pDIDevice->Acquire())){
		pDIDevice->Release();
		return NULL;
	}*/
	return pDIDevice;
}
IDirectInputDevice8* InitKeyboard(HWND hWnd, IDirectInput8 *pDI){
	IDirectInputDevice8 *pDIDevice;
	if(FAILED(pDI->CreateDevice(GUID_SysKeyboard,&pDIDevice, NULL)))
		return NULL;
	if(FAILED(pDIDevice->SetDataFormat(&c_dfDIKeyboard))) {
		pDIDevice->Release();
		return NULL;
	}
	if(FAILED(pDIDevice->SetCooperativeLevel(hWnd,
	DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		pDIDevice->Release();
		return NULL;
	}
	/*if(FAILED(pDIDevice->Acquire())) {
		pDIDevice->Release();
		return NULL;
	}
	if(FAILED(pDIDevice->GetDeviceState(sizeof(KeyStateBuffer),KeyStateBuffer))) 	 {
		pDIDevice->Release();
		return NULL;
	}*/
	return pDIDevice;
}
D3DXVECTOR3 handlingKeyboard(){
	pDIDKeyboard->Acquire();
	if(((pDIDKeyboard->GetDeviceState(sizeof(KeyStateBuffer), KeyStateBuffer)) == DIERR_NOTACQUIRED))
		pDIDKeyboard->Acquire();
	pDIDKeyboard->GetDeviceState(sizeof(KeyStateBuffer), KeyStateBuffer);
	D3DXVECTOR3 temp(0,0,0);
	if(KEYDOWN(KeyStateBuffer,DIK_LEFT)){
		temp.x = -SPEED;
	}
	if(KEYDOWN(KeyStateBuffer,DIK_RIGHT)){
		temp.x = SPEED;
	}
	if(KEYDOWN(KeyStateBuffer,DIK_DOWN)){
		temp.z = -SPEED*2;
	}
	if(KEYDOWN(KeyStateBuffer,DIK_UP)){
		temp.z = SPEED*2;
	}
	return temp;
}

bool mark;
D3DXVECTOR3 handlingMouse(D3DXVECTOR3& destination, D3DXVECTOR3& pEye, D3DXVECTOR3& pAt,D3DXVECTOR3& cursor){
	//char Text[256];
	pDIDMouse->Acquire();
	if((pDIDMouse->GetDeviceState(sizeof(DIMOUSESTATE),&MouseState))==DIERR_NOTACQUIRED){
		pDIDMouse->Acquire();
		g_MouseXPos=WNDWIDTH/2;
		g_MouseYPos=WNDHEIGHT/2; 
		g_MouseZPos=0;
	}
	g_MouseXPos+=MouseState.lX;
	g_MouseYPos+=MouseState.lY;
	g_MouseZPos+=MouseState.lZ;
	//проверки не позволяют выйти за границы окна(курсору игровому)
	if(g_MouseXPos<0 || g_MouseXPos>WNDWIDTH)
		g_MouseXPos=destination.x;
	else 
		destination.x=g_MouseXPos;
	if(g_MouseYPos<0 || g_MouseYPos>WNDHEIGHT)
		g_MouseYPos=destination.y;
	else 
		destination.y=g_MouseYPos;
			//обработка кнопок мыши.
	D3DXMATRIX view, proj, world;
	D3DVIEWPORT9 pViewport;
	D3DXVECTOR3 temp;
	if(MouseButtonState(MouseState,0)==true && mark==0){		//Левая кнопка
		temp.x=(destination.x-WNDWIDTH/2);	
		temp.y=(destination.y-WNDHEIGHT/2);
		mark=1;
	}
	if(MouseButtonState(MouseState,0)==false )
		mark=0;
	if(MouseButtonState(MouseState,1)==true){		//правая кнопка
		/*pEye.z+=0.1*(g_MouseXPos-WNDWIDTH/2);
		pEye.x+=0.1*(g_MouseYPos-WNDHEIGHT/2);
		pAt.z+=0.1*(g_MouseXPos-WNDWIDTH/2);
		pAt.x+=0.1*(g_MouseYPos-WNDHEIGHT/2);*/
	}
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &world);
	g_pD3DDevice->GetViewport(&pViewport);
	D3DXVec3Project(&temp, &D3DXVECTOR3(destination.x, destination.y, 0), &pViewport, &proj, &view, &world);
	cursor.x = temp.x-temp.y;
	cursor.y = 0;
	cursor.z = temp.z-temp.y;
	return cursor;
}

D3DXVECTOR3 cursorPosition(D3DXVECTOR3& destination){
	if ((pDIDMouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)) == DIERR_NOTACQUIRED){
		pDIDMouse->Acquire();
		g_MouseXPos = WNDWIDTH / 2;
		g_MouseYPos = WNDHEIGHT / 2;
		g_MouseZPos = 0;
	}
	g_MouseXPos += MouseState.lX;
	g_MouseYPos += MouseState.lY;
	g_MouseZPos += MouseState.lZ;
	//проверки не позволяют выйти за границы окна(курсору игровому)
	if (g_MouseXPos<0 || g_MouseXPos>WNDWIDTH)
		g_MouseXPos = destination.x;
	else
		destination.x = g_MouseXPos;
	if (g_MouseYPos<0 || g_MouseYPos>WNDHEIGHT)
		g_MouseYPos = destination.y;
	else
		destination.y = g_MouseYPos;
	return destination;
}
#endif /* DINPUTSYSTEM_H */

