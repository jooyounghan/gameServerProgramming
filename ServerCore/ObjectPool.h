#pragma once
#include "MemoryPool.h"

template <typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&& ...args)
	{
#ifdef _STOMP
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(OallocSize));
		Type* ptr = reinterpret_cast<Type*>(MemoryHeader::AttachHeader(header, OallocSize));
#else
		Type* ptr = reinterpret_cast<Type*>(MemoryHeader::AttachHeader(Opool.Pop(), OallocSize));
#endif
		new(ptr)Type(std::forward<Args>(args)...);
		return ptr;
	}
	
public:
	static void Push(Type* ptr)
	{
		ptr->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(ptr));
#else
		Opool.Push(MemoryHeader::DetachHeader(ptr););
#endif
	}
	static std::shared_ptr<Type> MakeShared()
	{
		std::shared_ptr<Type> ptr = { Pop(), Push };
		return ptr;
	}

public:
	static int32 OallocSize;
	static MemoryPool Opool;
};

template<typename Type>
int32 ObjectPool<Type>::OallocSize = static_cast<int32>(sizeof(Type)) + static_cast<int32>(sizeof(MemoryHeader));

template<typename Type>
MemoryPool ObjectPool<Type>::Opool{ OallocSize };

