#pragma once

/*
	VC++ Directories의 Include Directories와 Library Directories를 활용하여
	참조할 Precompiled Header를 탐색할 경로를 지정할 수 있다.
*/
#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif // _DEBUG


// add headers that you want to pre-compile here

#include "CorePch.h"
