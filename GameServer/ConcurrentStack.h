#pragma once

#include <mutex>
#include <atomic>

template<typename T>
class LockStack {
public:
	LockStack() {};
	LockStack(const LockStack&) = delete;
	LockStack& operator= (const T&) = delete;

	void Push(T value) {
		std::lock_guard<std::mutex> lockguard(m_mutex);
		m_lockstack.push(std::move(value));
		m_cv.notify_one();
	}

	bool TryPop(T& value) {
		std::lock_guard<std::mutex> lockguard(m_mutex);
		if (m_lockstack.size()) {
			value = std::move(m_lockstack.top());
			m_lockstack.pop();
			return true;
		}
		else {
			return false;
		}
	}

	void WaitPop(T& value) {
		std::unique_lock<std::mutex> uniquelock(m_mutex);
		m_cv.wait(uniquelock, [this]() {return m_lockstack.size(); });
		value = std::move(m_lockstack.top());
		m_lockstack.pop();
	}

private:
	std::stack<T> m_lockstack;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

template <typename T>
class LockFreeStack {
public:

	struct Node;

	struct CountedNodePtr {
		int16 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node {
		Node(const T& value) : data(std::make_shared<T>(value)) {};
		std::shared_ptr<T> data;
		std::atomic<int32> internelCount = 0;
		CountedNodePtr next;
	};

	void Push(T value) {
		CountedNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1; 
	}

	void TryPop() {

	}

	void IsAtomic() {
		std::cout << std::atomic_is_lock_free(&m_Top) << std::endl;
		std::cout << "sizeof : " << sizeof(m_Top) << std::endl;
	}

private:
	std::atomic<CountedNodePtr> m_Top;
};
