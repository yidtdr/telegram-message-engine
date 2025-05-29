#include <cstddef>
#include <functional>
#include <future>

template<typename Result>
struct Task {
	std::promise<Result> promise;
	std::function<Result()> workload;
	size_t sequence_id;

	Task(std::promise<Result>&& p, std::function<Result()>&& wl, size_t id) :
		promise(std::move(p)), workload(std::move(wl)), sequence_id(id) {}

	Task() = default;

	Task(Task&&) = default;

	Task(const Task&) = delete;
};
