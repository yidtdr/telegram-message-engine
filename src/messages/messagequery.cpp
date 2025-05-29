#include "messagequery.h"

bool MessageQuery::matches(Message message) const noexcept {
	auto&& messageData = message.getData();
	if (data.sender.has_value()) {
		if (data.sender.value() != messageData.sender) {
			return false;
		}
	}
	if (data.contains.has_value()) {
		bool t = false;
		for (auto&& text : data.contains.value()) {
			if (messageData.contains(text)) {
				t = true;
				break;
			}
		}
		if (!t) {
			return false;
		}
	}
	if (data.hasAttachment.has_value()) {
		if (data.hasAttachment.value() != messageData.attachment) {
			return false;
		}
	}

	return true;
}
