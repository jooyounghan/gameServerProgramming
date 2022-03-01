#pragma once
#include "Types.h"
#include "Allocator.h"

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>

template<typename T>
using Vector = std::vector<T, StlAllocator<T>>;

template<typename T>
using List = std::list<T, StlAllocator<T>>;

template<typename T>
using Deque = std::deque<T, StlAllocator<T>>;

template<typename T>
using Queue = std::queue<T, Deque<T>>;

template<typename T>
using Stack = std::stack<T, Deque<T>>;

template<typename T, typename Container = Vector<T>, typename Pred = std::less<typename Container::value_type>>
using PriorityQueue = std::priority_queue<T, Container, Pred>;

template<typename Key, typename Value, typename Pred = std::less<Key>>
using Map = std::map<Key, Value, Pred, StlAllocator<std::pair<const Key, Value>>>;

template<typename Key, typename Pred = std::less<Key>>
using Set = std::set<Key, Pred, StlAllocator<Key>>;

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using UnorderedMap = std::unordered_map<Key, Value, Hash, KeyEq, StlAllocator<std::pair<Key, Value>>>;

template<typename Key, typename Hash = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using UnorderedSet = std::unordered_set<Key, Hash, KeyEq, StlAllocator<Key>>;

using String = std::basic_string<char, std::char_traits<char>, StlAllocator<char>>;

using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>>;

