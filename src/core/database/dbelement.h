#include <concepts>

template<typename T>
concept Record = requires(T record) {
	{ record.id() } -> std::convertible_to<int>;
};
