#pragma once
#include <stack>
#include <map>
#include <vector>

/*--------------------
	  DeadLockDetector
--------------------*/

class DeadLockDetector
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	std::unordered_map<const char*, int32>	_nameToId;
	std::unordered_map<int32, const char*>	_idToName;
	std::map<int32, std::set<int32>>		_lockHistory;

	Mutex _lock;

private:
	std::vector<int32> _discoredOrder;
	int32 _discoveredCount = 0;
	std::vector<bool> _finished;
	std::vector<int32> _parent;
};

