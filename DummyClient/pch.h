#pragma once

/*
	VC++ Directories�� Include Directories�� Library Directories�� Ȱ���Ͽ�
	������ Precompiled Header�� Ž���� ��θ� ������ �� �ִ�.
*/
#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif // _DEBUG


// add headers that you want to pre-compile here

#include "CorePch.h"
