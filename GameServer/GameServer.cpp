#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <string>

#include "ThreadManager.h"


#include "PlayerManager.h"
#include "AccountManager.h"

int main() {
	std::cout << "This is Main Server\n";
	HelloWorld();
	
	GThreadManager->Launch([=]
		{
			while (true)
			{
				std::cout << "PlayerThenAccount" << std::endl;
				GPlayerManager.PlayerThenAccount();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

		}
	);

	GThreadManager->Launch([=]
		{
			while (true)
			{
				std::cout << "AccountThenPlayer" << std::endl;
				GAccountManager.AccountThenPlayer();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

		}
	);
	GThreadManager->Join();
}