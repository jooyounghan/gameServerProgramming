#include "pch.h"
#include "DeadLockDetector.h"

void DeadLockDetector::PushLock(const char* name)
{
	LockGuard lockguard(_lock);

	int32 lockId;
	auto nameFind = _nameToId.find(name);
	// 해당하는 lock이 정보에 업을 경우
	if (nameFind == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = nameFind->second;
	}

	if (_lockStack.empty() == false)
	{
		int32 topId = _lockStack.top();
		if (topId != lockId)
		{
			std::set<int32>& history = _lockHistory[topId];
			if (history.find(lockId) == history.end())
			{
				_lockHistory[topId].insert(lockId);
				CheckCycle();
			}
		}
	}
	_lockStack.push(lockId);
	return;
}

void DeadLockDetector::PopLock(const char* name)
{
	LockGuard lockguard(_lock);

	if (_lockStack.empty())
	{
		CRASH("INVALID POP");
	}
	if (_lockStack.top() != _nameToId[name])
	{
		CRASH("INVALID POP");
	}
	_lockStack.pop();
}

void DeadLockDetector::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = std::vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = std::vector<bool>(lockCount, false);
	_parent = std::vector<int32>(lockCount, -1);
	
	for (int32 idx = 0; idx < lockCount; ++idx)
		Dfs(0);
	
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockDetector::Dfs(int32 index)
{
	if (_finished[index])
	{
		return;
	}

	_discoveredOrder[index] = _discoveredCount++;

	auto historyFind = _lockHistory.find(index);
	if (historyFind == _lockHistory.end())
	{
		_finished[index] = true;
		return;
	}
	
	std::set<int32>& historyNow = historyFind->second;
	for (auto later : historyNow)
	{
		if (_discoveredOrder[later] != -1)
		{
			_parent[later] = index;
			Dfs(later);
			continue;
		}

		if (_discoveredOrder[index] < _discoveredOrder[later])
			continue;

		if (_finished[later] == false)
		{
			printf("%s -> %s\n", _idToName[index], _idToName[later]);
			int32 now = index;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == later)
					break;
			}
			CRASH("DEADLOCK DETECTED");
		}
	}
	_finished[index] = true;
}
