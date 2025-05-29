#include "concurrent_queue.h"
#include <future>
#include <thread>

template<typename Result>
class Pipeline{
public:
	Pipeline(size_t workersAmount) {
		for (size_t i = 0; i < workersAmount; i++) {
			workers.push_back(std::jthread([this]{workerRoutine();}));
		}
	} 
	~Pipeline() {
		taskQueue.stop();
		for (auto& worker : workers) { if (worker.joinable()) { worker.join(); } };
	}

	std::future<Result> submitTask(std::function<Result()> function) {
		std::promise<Result> promise;
		auto future = promise.get_future();
		taskQueue.push(Task(
				std::move(promise),
				std::move(function), 
				std::move(sequenceCounter++)));
		return future;
	} 
private:
	void workerRoutine() {
		while (true) {
			auto task = taskQueue.pop();
			if (!task.workload) {
				return;
			}
	
			try {
				Result result = task.workload();
				task.promise.set_value(result);
			} catch(...) {
				std::lock_guard lock(exceptionMutex);
				exceptions.push_back(std::current_exception());
			}
		}
	}

	ConcurrentTaskQueue<Result> taskQueue;
	std::vector<std::jthread> workers;
	std::atomic<size_t> sequenceCounter{0};

	std::mutex exceptionMutex;
	std::vector<std::exception_ptr> exceptions;
};
