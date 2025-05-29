#include "task.h"
#include <condition_variable>
#include <mutex>
#include <queue>


template<typename Result>
class ConcurrentTaskQueue {
public:
	void push(Task<Result>&& task)  {
		std::lock_guard lock(mutex);
		queue.push(std::move(task));
		cv.notify_one();
	}

	Task<Result> pop() {
		std::unique_lock lock(mutex);
		cv.wait(lock, [this]{ return !queue.empty() || s; });
		if (s && queue.empty()) {
			return {};
		}
		auto task = std::move(queue.front());
		queue.pop();
		return task;
	} 
	void stop() {
		std::lock_guard lock(mutex);
		s = true;
		cv.notify_all();
	}
private:
	std::mutex mutex;
	std::queue<Task<Result>> queue;
	std::condition_variable cv;
	bool s = false;
};
