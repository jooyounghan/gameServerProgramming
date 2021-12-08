#pragma once
#include <mutex>


template <typename T>
class LockQueue
{
public:
	LockQueue(){}
	LockQueue(const LockQueue&) = delete;
	LockQueue& operator= (const LockQueue&) = delete;
	
	void Push(T value) {
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.push(std::move(value));
		_cv.notify_one();
	}
	
	bool Empty() {
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	bool TryPop(T& value) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (_queue.empty()) {
			return false;
		}
		else {
			value = std::move(_queue.front());
			_queue.pop();
			return true;
		}
	}

	void WaitPop(T& value) {
		std::unique_lock<std::mutex> uniqueLock(_mutex);
		_cv.wait(uniqueLock, [this]() {return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}

private:
	std::queue<T> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
}; 



template<typename T>
class LockFreeQueue {
public:
	struct Node {
		Node(const T& value) : data(std::make_shared<T>(value)), next(nullptr) {};
		std::shared_ptr<T> data;
		std::shared_ptr<Node> next;
	};

	LockFreeQueue() : m_front(std::make_shared<Node>(false)), m_rear(m_front) {};

	void Push(T value) {
		/*
			[dummy]
			front, rear

			[value] [dummy]
			front	rear
		*/

		std::shared_ptr<Node> dummy = std::make_shared<Node>(value);
		std::shared_ptr<Node> oldRear = m_rear;

	

	}



private:
	std::shared_ptr<Node> m_front;
	std::shared_ptr<Node> m_rear;
};