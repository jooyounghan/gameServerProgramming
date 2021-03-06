#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	// Pool에 메모리 반납
	WRITE_LOCK;
	ptr->allocSize = 0;
	_queue.push(ptr);
	
	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	// Pool에서 사용 가능한 메모리 사용
	MemoryHeader* header = nullptr;
	{
		WRITE_LOCK;
		if (_queue.empty() == false)
		{
			header = _queue.front();
			_queue.pop();
		}
	}

	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}
	else
	{
		ASSERT_CRASH(header->allocSize == 0);
	}
	_allocCount.fetch_add(1);
	return header;
}