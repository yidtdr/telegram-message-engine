#include "messages/message.h"
#include "core/parsenode/node.h"

class MessageExtractor {
public:
	std::vector<Message> extract(GumboNode* document);
private:
	std::vector<Node> findAllMessages(Node& document) const;
	Message getMessage(Node& message);
	std::string getSender(Node& message);
	std::string getContent(Node& message);
	std::chrono::system_clock::time_point getTimestamp(Node& message);
	bool getAttachment(Node& message);

	Selector CONTENT_SELECTOR{.cl = "text"};
	Selector MESSAGE_SELECTOR{.cl = "message default"};
	Selector TIMESTAMP_SELECTOR{.tag = "div", .cl = "date"};
	Selector SENDER_SELECTOR{.cl = "from_name"};
	Selector ATTACHMENT_SELECTOR{.cl = "from_name"};
	Selector HISTORY_SELECTOR{.cl = "history"};
public:
	MessageExtractor(const MessageExtractor&) = delete;
	MessageExtractor& operator=(const MessageExtractor&) = delete;

	static MessageExtractor& getInstance() {
		static MessageExtractor instance;
		return instance;
	}
private:
	MessageExtractor() = default;
	~MessageExtractor() = default;
};
