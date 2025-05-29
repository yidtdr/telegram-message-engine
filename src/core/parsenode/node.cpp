#include "node.h" 

std::string Node::getTag() const {
	return gumbo_normalized_tagname(node->v.element.tag);
}

std::string Node::getAttribute(const std::string name) const {
	if (node->type != GUMBO_NODE_ELEMENT) {
		return "";
	}
	GumboAttribute* attr = gumbo_get_attribute(&node->v.element.attributes, name.c_str());

	return attr ? attr->value : "";
}

std::string Node::getText() const {
	if (node->type != GUMBO_NODE_TEXT) {
		if (node->type != GUMBO_NODE_ELEMENT) {
			return "";
		}
		else {
			std::string result;
			const auto children = &node->v.element.children;
			for (size_t i = 0; i < children->length; i++) {
				GumboNode* child = static_cast<GumboNode*>(children->data[i]);
				if (child->type == GUMBO_NODE_TEXT) {
					result.append(child->v.text.text);
				}
			}
			return result;
		}
	}
	return node->v.text.text;
}

inline bool Node::matchesSelector(const Selector& selector) {
	if (node->type != GUMBO_NODE_ELEMENT) {
		return false;
	}

	if (selector.id.has_value()) {
		if (selector.id.value() != getAttribute("id")) {
			return false;
		}
	}
	
	if (selector.tag.has_value()) {
		if (selector.tag.value() != getTag()) {
			return false;
		}
	}

	if (selector.cl.has_value()) {
		if (getAttribute("class").find(selector.cl.value()) == std::string::npos) {
			return false;
		}
	}
	
	return true;
}

std::vector<Node> Node::querySelectorAll(const Selector& selector, size_t depth, size_t reserve) { 
	std::vector<Node> result; 
	if (reserve > 0) {
		result.reserve(reserve);
	}
	const GumboVector* children = &node->v.element.children; 
	for (size_t i = 0; i < children->length; i++) { 
		GumboNode* child = static_cast<GumboNode*>(children->data[i]); 
		if (child->type != GUMBO_NODE_ELEMENT) {
			continue;
		}
		Node child_node(child);
		if (child_node.matchesSelector(selector)) {
			result.push_back(child_node);
		}

		if (depth > 0) {
			auto child_results = child_node.querySelectorAll(selector, depth-1, 0);
			result.reserve(result.size() + child_results.size());
			for (auto& result_node : child_results) {
				result.push_back(std::move(result_node));
			}
		}
	}
	return result;
}

std::optional<Node> Node::querySelector(const Selector& selector) {
	if (node->type != GUMBO_NODE_ELEMENT) {
		return std::nullopt;
	}
	const auto children = &node->v.element.children;
	for (size_t i = 0; i < children->length; i++) {
		GumboNode* child = static_cast<GumboNode*>(children->data[i]);
		Node child_node(child);
		if (child_node.matchesSelector(selector)) {
			return child_node;
		}
		const auto queryResults = child_node.querySelector(selector);
		if (queryResults.has_value()) {
			return queryResults;
		}
	}
	return std::nullopt;
}
