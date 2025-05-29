#include <core/database/database.h>
#include "message.h"
#include <optional>
#include <vector>
#include <string>
#include <chrono>

struct MessageQueryParameters {
	std::optional<std::string> sender;
	std::optional<std::vector<std::string>> contains;
	std::optional<std::chrono::system_clock::time_point> timeFrom;
	std::optional<std::chrono::system_clock::time_point> timeTo;
	std::optional<bool> hasAttachment;
};

class MessageQuery {
public:
	MessageQuery(MessageQueryParameters&& data) : data(std::move(data)) {};
	bool matches(Message message) const noexcept;
private:
	MessageQueryParameters data;
};
