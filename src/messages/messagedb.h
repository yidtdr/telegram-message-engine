#include "messagequery.h"

class MessageDatabase : public Database<MessageDatabase, Message> {
public:
	void insert(Message message) override;
	std::vector<Message> selectImpl(MessageQuery query) const;
	std::vector<Message> getMessages() const;
	void reserve(size_t amount);

public:
	MessageDatabase(const MessageDatabase&) = delete;
	MessageDatabase& operator=(const MessageDatabase&) = delete;

	static MessageDatabase& getInstance() {
		static MessageDatabase instance;
		return instance;
	}

private:
	MessageDatabase() = default;
	~MessageDatabase() = default;
};
