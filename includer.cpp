#include "includer.h"

#ifndef INCLUDER_CPP
#define INCLUDER_CPP
void Error(const char* error){
	MessageBox(NULL, (LPCTSTR)error, (LPCTSTR)"Error", MB_OK);
}
#endif /*INCLUDER_CPP*/