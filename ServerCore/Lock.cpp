#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockDetector.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockDetector->PushLock(name);
#endif

	// 재귀적인 Lock을 걸어줄 경우 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}
	
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	const uint32 desired = (LThreadId << 16) & WRITE_THREAD_MASK;
	
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			// compare_exchange_strong의 코드에 의해 expected의 값이 계속 바뀌므로
			// for문 내부에서 정의해주어야한다.
			uint32 expected = EMPTY_FLAG;
			if(_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}
		uint32 timeDelay = (std::chrono::system_clock::now() - startTime).count();
		if (timeDelay >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		std::this_thread::yield();
	}

}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockDetector->PopLock(name);
#endif

	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockDetector->PushLock(name);
#endif

	const uint32 lockThreadId = _lockFlag.load() & WRITE_THREAD_MASK;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = _lockFlag.load() & READ_COUNT_MASK;
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
			{
				return;
			}
		}
		
		uint32 timeDelay = (std::chrono::system_clock::now() - startTime).count();
		if (timeDelay >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		std::this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockDetector->PopLock(name);
#endif
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
