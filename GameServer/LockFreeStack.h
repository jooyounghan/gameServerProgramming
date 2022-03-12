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
	// ������ SListEntry(���� ��)�� ������ �ϴ� �����͸� �ϳ� �̷��, ���� ���� �����͸� SListEntry�� ����
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