#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <string>

#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"
#include "Memory.h"

class Knight
{
public:
	int32 _hp = 1000;
	int32 _mp = 200;

public:
	Knight() { printf("I'm Knight!\n"); };
	Knight(const int32& hp_in, const int32& mp_in) : _hp(hp_in), _mp(mp_in) { printf("I'm Knight!\n"); };
	~Knight() { printf("GoodBye From Knight\n"); }
};

int main() {
	std::cout << "This is Main Server\n";
	HelloWorld();

	Knight* knight = xnew<Knight>(10, 5);

	std::vector<Knight, StlAllocator<Knight>> a(10);


	for (int32 i = 0; i < 10; ++i)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Vector<Knight> v(10);
					Map<int32, Knight> a;
					a[100] = Knight();
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			});
	}
	GThreadManager->Join();
}