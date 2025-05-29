#include "message_extractor.h"
#include <algorithm>
#include <sstream>
#include <utility>

std::vector<Message> MessageExtractor::extract(GumboNode* document) {
	std::vector<Message> messages;
	Node docNode(document);
	const auto messageNodes = findAllMessages(docNode);
	for (auto messageNode : messageNodes) {
		messages.push_back(std::move(getMessage(messageNode)));
	}
	return messages;
}

std::vector<Node> MessageExtractor::findAllMessages(Node& document) const {
	auto history = document.querySelector(HISTORY_SELECTOR);
	if (!history.has_value()) {
		return std::vector<Node>();
	}
	return history.value().querySelectorAll(MESSAGE_SELECTOR, 0, 1000);
}

std::string MessageExtractor::getSender(Node& message) {
	const auto senderNode = message.querySelector(SENDER_SELECTOR);
	if (senderNode.has_value()) {
		std::string senderString = senderNode.value().getText();
		senderString.erase(std::remove_if (senderString.begin(), senderString.end(), [](char c) {
					return c == '\n' || c == '\t' || c == ' ';
					}), senderString.end());
		return senderString;
	}
	return "";
}

std::string MessageExtractor::getContent(Node& message) {
	std::string result = "";
	for (const auto content : message.querySelectorAll(CONTENT_SELECTOR)) {
		result.reserve(result.size() + content.getText().size());
		result.append(std::move(content.getText()));
	}
	return result;
}

bool MessageExtractor::getAttachment(Node& message) {
	return !message.querySelectorAll(ATTACHMENT_SELECTOR).empty();
}

std::chrono::system_clock::time_point MessageExtractor::getTimestamp(Node& message) {
    auto timestampNode = message.querySelector(TIMESTAMP_SELECTOR);
    if (!timestampNode.has_value()) {
        throw std::runtime_error("Timestamp node has no value!");
    }

    std::string timeString = timestampNode.value().getAttribute("title");
    std::istringstream iss(timeString);
    
    std::tm tm = {};
    char offset[7] = {};
    iss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");
    iss >> offset;  // reads "UTC+05:00"
    
    if (iss.fail()) {
        throw std::runtime_error("Failed to parse timestamp");
    }
    
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    
    int hours, minutes;
    if (sscanf(offset + 3, "%d:%d", &hours, &minutes) == 2) {
        auto offset_duration = std::chrono::hours(hours) + std::chrono::minutes(minutes);
        if (offset[3] == '+') {
            tp -= offset_duration;  // convert to UTC
        } else {
            tp += offset_duration;  // convert to UTC
        }
    }
    
    return tp;
}


Message MessageExtractor::getMessage(Node& message) {
	return Message(MessageData{
	.id = 0,
	.sender = getSender(message),
	.content = getContent(message),
	.timestamp = getTimestamp(message),
	.attachment = getAttachment(message)
	});
}
