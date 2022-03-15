#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <string>

#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"
#include "Memory.h"

using TL = TypeList<class Animal, class Human, class Wolf, class Knight>;

class Animal
{
public:
	DECLARE_TL
	Animal() { INIT_TL(Animal) }
};

class Human
{
public:
	DECLARE_TL
	Human() { INIT_TL(Human) }
};

class Wolf : public Animal
{
public:
	Wolf() { INIT_TL(Wolf) }
	int32 _hp = 500;
	int32 _mp = 100;
};

class Knight : public Human
{
public:
	int32 _hp = 1000;
	int32 _mp = 200;

public:
	Knight() { INIT_TL(Knight); printf("I'm Knight!\n"); };
	Knight(const int32& hp_in, const int32& mp_in) : _hp(hp_in), _mp(mp_in) { printf("I'm Knight!\n"); };
	~Knight() { printf("GoodBye From Knight\n"); }
};

int main() {
	std::cout << "This is Main Server\n";
	HelloWorld();


	Animal* animal = new Animal();
	bool canCast = CanCast<Wolf*>(animal);
	Wolf* wolf = TypeCast<Wolf*>(animal);

	wolf = new Wolf();
	bool canCast2 = CanCast<Animal*>(wolf);
	Animal* newAnimal = TypeCast<Animal*>(wolf);

	std::shared_ptr<Animal> sharedanimal = MakeShared<Animal>();
	std::shared_ptr<Wolf> sharedwolf = TypeCast<Wolf>(sharedanimal);

	sharedwolf = MakeShared<Wolf>();
	std::shared_ptr<Animal> sharednewAnimal = TypeCast<Animal>(sharedwolf);
	//for (int32 i = 0; i < 10; ++i)
	//{
	//	GThreadManager->Launch([]()
	//		{
	//			while (true)
	//			{
	//				std::shared_ptr<Knight> ptr = ObjectPool<Knight>::MakeShared();
	//				std::shared_ptr<Knight> ptr2 = MakeShared<Knight>();
	//				std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//			}
	//		});
	//}
	//GThreadManager->Join();
}