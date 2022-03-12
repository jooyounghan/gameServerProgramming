#pragma once
#include "pch.h"
/* ----------------------
	  BaseAllocator
---------------------- */

class BaseAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};

/* ----------------------
	  StompAllocator
---------------------- */
class StompAllocator
{
	// 일반적으로 4KB를 하나의 페이지 단위로 구분하여 사용한다.
public:
	// enum을 활용하여 멤버변수를 사용한 이유는 static 함수를 활용하기 위해서이다.
	enum { PAGE_SIZE = 0x1000 };

public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};

/* ----------------------
	  PoolAllocator
---------------------- */
class PoolAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};


/* ----------------------
	  STL Allocator
---------------------- */
template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {};

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(sizeof(T) * count);
		return static_cast<T*>(XALLOC(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		XRELEASE(ptr);
	}

};