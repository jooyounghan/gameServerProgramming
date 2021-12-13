#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <string>

#include "ThreadManager.h"

CoreGlobal Core;

void ThreadMain() {
	while (true) {
		std::cout << "Hello I'm Thread number : " << LThreadId << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main() {
	std::cout << "This is Main Server\n";
	HelloWorld();
	
	for (int32 i = 0; i < 5; ++i) {
		GThreadManager->Launch(ThreadMain);
	}
	GThreadManager->Join();
}