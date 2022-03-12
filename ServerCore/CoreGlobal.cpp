#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "Memory.h"

ThreadManager* GThreadManager = nullptr;
DeadLockDetector* GDeadLockDetector = nullptr;
Memory* GMemory = nullptr;

class CoreGlobal
{
public:
	CoreGlobal() {
		GThreadManager = new ThreadManager();
		GDeadLockDetector = new DeadLockDetector();
		GMemory = new Memory();
	}

	~CoreGlobal() {
		delete GThreadManager;
		delete GDeadLockDetector;
		delete GMemory;
	}
} Core;
