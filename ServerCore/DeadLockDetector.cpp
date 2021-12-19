#include "pch.h"
#include "DeadLockDetector.h"

void DeadLockDetector::PushLock(const char* name)
{
	LockGuard guard(_lock);

	int32 lockId = 0;

	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	if (LLockStack.empty() == false)
	{
		const int32 previd = LLockStack.top();
		if (lockId != previd)
		{
			std::set<int32>&history = _lockHistory[previd];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}
	LLockStack.push(lockId);
}

void DeadLockDetector::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	LLockStack.pop();

}

void DeadLockDetector::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoredOrder = std::vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = std::vector<bool>(lockCount, false);
	_parent = std::vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	_discoredOrder.clear();
	_finished.clear();
	_parent.clear();

}

void DeadLockDetector::Dfs(int32 here)
{
	if (_discoredOrder[here] != -1)
		return;

	_discoredOrder[here] = _discoveredCount++;

	auto findIt = _lockHistory.find(here);
	if (findIt == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	std::set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		if (_discoredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		if (_discoredOrder[here] < _discoredOrder[there])
			continue;

		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);

			int32 now = here;
			while (true) {
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}
