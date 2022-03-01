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
	// �Ϲ������� 4KB�� �ϳ��� ������ ������ �����Ͽ� ����Ѵ�.
public:
	// enum�� Ȱ���Ͽ� ��������� ����� ������ static �Լ��� Ȱ���ϱ� ���ؼ��̴�.
	enum { PAGE_SIZE = 0x1000 };

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