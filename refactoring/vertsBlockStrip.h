#include <fstream>
#include "d3d9.h"
#include "d3dx9.h"
#include "D3dx9core.h"
#include <fstream>

typedef struct {
	FLOAT x, y, z;
	//D3DCOLOR diffuse;
	FLOAT u, v;
} sVertex;

#define VERTEXFVF (D3DFVF_XYZ| D3DFVF_TEX1)

extern IDirect3DDevice9 *g_pD3DDevice;
extern IDirect3D9       *g_pD3D;

const long int numberOfLine=75;
const long int numberOfBlocks=325;						//число блоков(на один экран 64 блока.Создадим 10 экранов)
const long int numberOfVerts=numberOfBlocks*4;	//число вершин(на первый блок 4, на последующие по 2 вершины)
const long int dx=48;				//dx - шаг вершины.
const long int xMax=1024;			//крайний по x в первом октанте

using namespace std;
class vertsBlockStrip{
private:
	
public:
	
	sVertex* stripVerts1;
	IDirect3DTexture9* texture1;
	long int numberOfTexture;	//число текстур
	long int numberOfColumn;	//столбцы в текстурах
	long int blockTextureSize;	//размер одной текстуры
	
	IDirect3DTexture9* texture;
	IDirect3DVertexBuffer9* vertexStripBuf;
	IDirect3DVertexBuffer9* longBufVertex;
	
	
	//в ширину текстуры
	bool selectTexture(const int& numb, sVertex* vert){
		if(numb>=numberOfTexture || numb<0)
			throw "error";	
		(vert+0)->u=numb/(float)numberOfTexture;
		(vert+0)->v=1.0f;
		
		(vert+1)->u=numb/(float)numberOfTexture;
		(vert+1)->v=0.0f;
		
		(vert+2)->u=(numb+1)/(float)numberOfTexture;
		(vert+2)->v=1.0f;
		
		(vert+3)->u=(numb+1)/(float)numberOfTexture;
		(vert+3)->v=0.0f;
	}
	/*IDirect3DVertexBuffer9* vertexStripBuf2;
	IDirect3DVertexBuffer9* vertexStripBuf3;
	IDirect3DVertexBuffer9* vertexStripBuf4;
	IDirect3DVertexBuffer9* vertexStripBuf5;
	IDirect3DVertexBuffer9* vertexStripBuf6;
	IDirect3DVertexBuffer9* vertexStripBuf7;
	IDirect3DVertexBuffer9* vertexStripBuf8;
	IDirect3DVertexBuffer9* vertexStripBuf9;
	IDirect3DVertexBuffer9* vertexStripBuf10;
	IDirect3DVertexBuffer9* vertexStripBuf11;
	IDirect3DVertexBuffer9* vertexStripBuf12;
	IDirect3DVertexBuffer9* vertexStripBuf13;
	IDirect3DVertexBuffer9* vertexStripBuf14;
	IDirect3DVertexBuffer9* vertexStripBuf15;
	IDirect3DVertexBuffer9* vertexStripBuf16;
	IDirect3DVertexBuffer9* vertexStripBuf17;*/
	
	/*	//При использовании не позволяет определять дальнейшую текстуру.Нужно разобраться почему,
		//а пока что используем объявление и определение ч/з getVerts() отдельно
	vertsBlockStrip(const int& zMax, const int& dz){
		stripVerts=new sVertex[numberOfVerts];
		long int temp=xMax;
		int count=0;
		while(count<numberOfVerts){
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
		}
	}*/
	/* getVerts(const int& zMax, const int& dz){
		stripVerts=new sVertex[numberOfVerts];
		long int temp=xMax;
		int count=0;
		while(count<numberOfVerts){
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
		}
	}*/
	
	IDirect3DTexture9* loadTextureFromFile(char* filePlace,const UINT& width,const UINT& height,const int& textNumb,const int& columnNumb){
		LPCTSTR temp=(LPCTSTR)filePlace;
		D3DXCreateTextureFromFileEx(g_pD3DDevice,temp,width,height,D3DX_DEFAULT,0,D3DFMT_FROM_FILE,D3DPOOL_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_RGBA(0,0,0,255),0,NULL,&texture);
		numberOfTexture = textNumb;
		numberOfColumn 	= columnNumb;
		blockTextureSize= width/columnNumb;
		return texture;
	}
	/*
	bool textureFill(//std::fstream& input
	){
		int count=0;
		int temt=0;
	
		while(temt<numberOfBlocks){
			stripVerts[count].u=0.5f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].u=0.5f;
			stripVerts[count].v=0.0f;
			count++;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=0.0f;
			count++;
			temt++;
		}
		return true;
	}
	*/
	
	IDirect3DVertexBuffer9* superBufCreater(std::fstream& input){
		input.seekg(0,ios::beg);
		float zMax(48.0f);						
		sVertex stripVerts[numberOfLine][numberOfVerts];	
		long int temp;
		int numText=0;
		for(int i=numberOfLine-1;i>=0;i--){
			temp=xMax;
			//массив должен быть кратен 4(иначе out_of_range !)
			for(int count=0;count<numberOfVerts;){
				input>>numText;
				if(numText>=numberOfTexture || numText<0 || input.eof())
					throw "error";
				else
					selectTexture(numText,&(stripVerts[i][count]));
				stripVerts[i][count].x=temp;
				stripVerts[i][count].z=zMax;
				stripVerts[i][count].y=0.0f;
				count++;
				stripVerts[i][count].x=temp;
				stripVerts[i][count].z=zMax-dx;
				stripVerts[i][count].y=0.0f;
				count++;
				temp-=dx;
				stripVerts[i][count].x=temp;
				stripVerts[i][count].z=zMax;
				stripVerts[i][count].y=0.0f;
				count++;
				stripVerts[i][count].x=temp;
				stripVerts[i][count].z=zMax-dx;
				stripVerts[i][count].y=0.0f;
				count++;
			}
			zMax+=dx;
		}
		BYTE* ptr;
		g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*numberOfLine*numberOfVerts, 0,VERTEXFVF, D3DPOOL_DEFAULT,&longBufVertex, NULL);
		longBufVertex->Lock(0,0, (void**)&ptr, 0);
		for(int i=0;i<numberOfLine;i++){
			memcpy(ptr, &stripVerts[i], sizeof(sVertex)*numberOfVerts);
			ptr+=sizeof(sVertex)*numberOfVerts;
		}
		ptr=NULL;
		longBufVertex->Unlock();
		
		/*Рабочий вариант, в буфер заносим 2 линии вершин (3:15)
		float zMax(768.0f),dz(100.0f);
		sVertex stripVerts[numberOfVerts];
		long int temp=xMax;
		int count=0;
		while(count<numberOfVerts){
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=0.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=1.0f;
			count++;
			stripVerts[count].x=temp;
			stripVerts[count].z=zMax-dz;
			stripVerts[count].y=0.0f;
			stripVerts[count].u=1.0f;
			stripVerts[count].v=0.0f;
			count++;
			temp-=dx;
		}
		zMax-=dz;
		sVertex stripVerts1[numberOfVerts];
		temp=xMax;
		count=0;
		while(count<numberOfVerts){
			stripVerts1[count].x=temp;
			stripVerts1[count].z=zMax;
			stripVerts1[count].y=0.0f;
			stripVerts1[count].u=0.0f;
			stripVerts1[count].v=1.0f;
			count++;
			stripVerts1[count].x=temp;
			stripVerts1[count].z=zMax-dz;
			stripVerts1[count].y=0.0f;
			stripVerts1[count].u=0.0f;
			stripVerts1[count].v=0.0f;
			count++;
			temp-=dx;
			stripVerts1[count].x=temp;
			stripVerts1[count].z=zMax;
			stripVerts1[count].y=0.0f;
			stripVerts1[count].u=1.0f;
			stripVerts1[count].v=1.0f;
			count++;
			stripVerts1[count].x=temp;
			stripVerts1[count].z=zMax-dz;
			stripVerts1[count].y=0.0f;
			stripVerts1[count].u=1.0f;
			stripVerts1[count].v=0.0f;
			count++;
			temp-=dx;
		}
		BYTE* ptr;
		g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*2*numberOfVerts, 0,VERTEXFVF, D3DPOOL_DEFAULT,&vertexStripBuf, NULL);
		vertexStripBuf->Lock(0,0, (void**)&ptr, 0);
		memcpy(ptr, stripVerts, sizeof(sVertex)*numberOfVerts);
		ptr+=sizeof(sVertex)*numberOfVerts;
		memcpy(ptr, stripVerts1, sizeof(sVertex)*numberOfVerts);
		vertexStripBuf->Unlock();
		*/
		/*g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*numberOfVerts, 0,VERTEXFVF, D3DPOOL_DEFAULT,&vertexStripBuf1, NULL);
		vertexStripBuf1->Lock(0,0, (void**)&ptr, 0);
		memcpy(ptr, stripVerts1, sizeof(sVertex)*numberOfVerts);
		vertexStripBuf1->Unlock();
		return vertexStripBuf;*/
	}
	
	
	bool DrawPrimitive(){
		g_pD3DDevice->SetStreamSource(0, longBufVertex, 0, sizeof(sVertex));
		g_pD3DDevice->SetFVF(VERTEXFVF);
		g_pD3DDevice->SetTexture(0, texture);
		for(int i=0;i<numberOfLine;i++){
			g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*numberOfVerts, numberOfBlocks*2);
		}
		/*//Рабочий вариант, вывод 2-х строк вершин. (3:15)
		g_pD3DDevice->SetStreamSource(0, longBufVertex, 0, sizeof(sVertex));
		g_pD3DDevice->SetTexture(0, texture);
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, numberOfBlocks*2);
		*/

	}
	/*
	IDirect3DVertexBuffer9* createBufForBlockStrip(){
		BYTE* ptr;
		g_pD3DDevice->CreateVertexBuffer(sizeof(sVertex)*numberOfVerts, 0,VERTEXFVF, D3DPOOL_DEFAULT,&vertexStripBuf, NULL);
		vertexStripBuf->Lock(0,0, (void**)&ptr, 0);
		memcpy(ptr, stripVerts, sizeof(stripVerts[0])*numberOfVerts);
		vertexStripBuf->Unlock();
		return vertexStripBuf;
		
	}*/
	
	
		/*
		1)функция загрузки текстур в память,для дальнейшей работы с ней(пока один файл)
		2)По номеру текстуры определить её координаты в файле
		*/
	};