#pragma once



struct SListEntry
{
	SListEntry* next;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};

class Data
{
public:
	SListEntry _entry;
	int32 _hp;
	int32 _mp;
};

class LockFreeStack
{
};

void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	// 기존에 SListEntry(가장 앞)의 역할을 하던 데이터를 하나 미루고, 새로 들어온 데이터를 SListEntry로 설정
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;
	
	if (first != nullptr)
		header->next = first->next;

	return first;
}