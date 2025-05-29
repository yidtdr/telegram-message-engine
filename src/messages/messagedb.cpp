#include "messagedb.h"

void MessageDatabase::insert(Message message) {
	elements.push_back(message);
}

std::vector<Message> MessageDatabase::selectImpl(MessageQuery query) const {
	std::vector<Message> result;
	for (const auto& message : elements) {
		if (query.matches(message)) {
			result.push_back(std::move(message));
		}
	}
	return result;
}

std::vector<Message> MessageDatabase::getMessages() const {
	return elements;
}

void MessageDatabase::reserve(size_t amount) {
	elements.reserve(elements.size() + amount);
}
