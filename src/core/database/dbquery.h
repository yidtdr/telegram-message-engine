#include <concepts>
#include "dbelement.h"

template<typename S, typename R>
concept Query = requires(S filter, R element) {
	{ filter.matches(element) } -> std::convertible_to<bool>;
	requires Record<R>;
};
