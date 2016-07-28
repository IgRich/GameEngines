#include "AnimBoard.h"

AnimBoard::AnimBoard(){
	animStage = NULL;
	animInterrupt = false;
	curStage = curStage = tempTime = animFrame = 0;
}
AnimBoard::~AnimBoard(){
	if (animStage != NULL){
		delete[] animStage;
		animStage = NULL;
	}
}
bool AnimBoard::descriptReader(std::fstream& file){
	Board::descriptReader(file);
	int animNumb;
	file >> animNumb;
	int* array = new int[animNumb];
	for (int i = 0; i<animNumb; i++)
		file >> array[i];
	if (!setAnimStage(array, animNumb))
		Error("stage error!");
	delete[] array;
	return true;
}
//ниже дичь написана!
bool AnimBoard::descriptReader(const descriptor& desc){
	D3DXMatrixIdentity(&worldMat);
	if (!setAnimStage(desc.array, desc.animNumb))
		Error("stage error!");
	if (desc.boardHeight <= 0 || desc.boardHeight <= 0)
		return false;
	bas = new ImageBasis;
	bas->setParam(desc.filePlace, desc.boardWidth, desc.boardHeight, desc.numb, desc.column, desc.line, desc.animNumb,desc.array);
	return true;
}
bool AnimBoard::setAnimStage(const int* array, const int& animNumb){
	numbOfStage = animNumb;
	animStage = new int[2*animNumb+1];
	animStage[0] = 0;
	for (int i = 1, count = 0; i < (2 * animNumb + 1);count++){
		animStage[i] = animStage[i-1] + (array[count+1] - array[count]);
		i++;
		animStage[i] = animStage[i-2] + 2*(array[count+1] - array[count]);
		i++;
	}
	return true;
}
bool AnimBoard::setStage(const int& numb,const bool& left){
	if (numb<0 || numb>numbOfStage)	//если кривое значение номера анимации
		return false;
	if (animInterrupt == true)		//если объект зан€т
		return false;
		//иначе устанавливаем анимацию
	//left=true  -	дефолтные настройки.“о етсь на самой картинке перса влево повЄрнуть нужно
	if (left == true){
		prevStage = curStage;
		curStage = numb + numb;
	}
	else{
		prevStage = curStage;
		curStage = numb + numb + 1;
	}
	return true;
}
bool AnimBoard::interrupt(const int& numb, const bool& left){
	if (animInterrupt == true)	//если объект уже зан€т
		return false;
	else{
		setStage(numb, left);
		animInterrupt = true;
		animFrame =  animStage[curStage];
		return true;
	}
}
bool AnimBoard::draw(){
	//сохранение изначальной матрицы мира,дл€ последующего восстановлени€
	D3DXMATRIX matWorldSaved;
	g_pD3DDevice->GetTransform(D3DTS_WORLD, &matWorldSaved);
			//*анимаци€ объекта*//
	/*обработка прерывани€(рисование отдельной анимации)*/
	if (animInterrupt == true){
		if ((clock() / defaultSpeed(0.25)) - tempTime >= 1){
			tempTime = clock() / defaultSpeed(0.25);
			if (animFrame < animStage[curStage + 1] - 1)
				animFrame++;
			else{
				animInterrupt = false;
				int tmp = curStage;
				curStage = prevStage;
				prevStage = curStage;
			}
		}
	}
	else{
		/*иначе рисуетс€ та, котора€ без приоритетов*/
		if (prevStage != curStage){						//если сменили на новую, то заного начинаем отрисовку
			tempTime = clock() / defaultSpeed(0.25);
			animFrame = animStage[curStage];
		}
		if ((clock()/defaultSpeed(0.25)) - tempTime>= 1){
			tempTime = clock() / defaultSpeed(0.25);
			if (animFrame < animStage[curStage+1] - 1)
				animFrame++;
			else
				animFrame = animStage[curStage];
		}
	}
		//*конец алгоритма анимации*//
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMat);
	g_pD3DDevice->SetTexture(0,bas->getTexture());
	g_pD3DDevice->SetStreamSource(0, bas->getBuf(), 0, sizeof(sVertex));
	g_pD3DDevice->SetFVF(VERTEXFVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,4*animFrame, 2);//рисуем одну картонку
	g_pD3DDevice->SetTexture(0, NULL);								 //очищаем текстуру
	g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(sVertex));	 //очищаем поток вершин
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldSaved);			 //вернуть исходную не забываем!

	return true;
}

bool AnimBoard::copy(Board* obj){
		//setAnimStage_copy
	numbOfStage = dynamic_cast<AnimBoard*>(obj)->numbOfStage;
	animStage = new int[2*numbOfStage+1];
	animStage[0] = 0;
	for (int i=1;i<(2*numbOfStage+1);i++)
		animStage[i] = dynamic_cast<AnimBoard*>(obj)->animStage[i];
	bas = getPointer(dynamic_cast<AnimBoard*>(obj)->bas);
	return true;
}