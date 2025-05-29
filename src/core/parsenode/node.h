#include <climits>
#include <gumbo.h>
#include <optional>
#include <string>
#include <vector>

struct Selector{
	std::optional<std::string> tag = std::nullopt;
	std::optional<std::string> cl = std::nullopt;
	std::optional<std::string> id = std::nullopt;
};

class Node {
public:
	Node(const GumboNode* node): node(node) {};

	std::string getTag() const;
	std::string getAttribute(const std::string name) const;
	std::string getText() const;
	std::vector<Node> querySelectorAll(const Selector& selector, size_t depth = INT_MAX, size_t reserve = 0);
	std::optional<Node> querySelector(const Selector& selector);
	inline bool matchesSelector(const Selector& selector);
private:
	const GumboNode* node;
};
