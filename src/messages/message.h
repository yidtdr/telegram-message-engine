#pragma once
#include <string>
#include <chrono>

#include <utils/string_helper.h>

struct MessageData{
	size_t id;
	std::string sender;
	std::string content;	
	std::chrono::system_clock::time_point timestamp;
	bool attachment;

	bool contains(const std::string& word) const {
		return (StringHelper::case_insensitive_find(content, word) != std::string::npos);
	}
};

class Message{
public:
	Message(MessageData&& data) : data(std::move(data)) {};
	size_t id() const noexcept { return data.id; };
	const MessageData& getData() const noexcept { return std::move(data); };
private:
	MessageData data;
};
