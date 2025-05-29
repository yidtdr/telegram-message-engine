#include <vector>
#include "dbquery.h"

template<typename Derived, Record RecordType>
class Database {
public:
	virtual void insert(RecordType element) = 0;
	
	template<typename QueryType>
		requires Query<QueryType, RecordType>
	std::vector<RecordType> select(QueryType&& query) const {
		return static_cast<const Derived*>(this)->selectImpl(std::move(query));
	};
protected:
	std::vector<RecordType> elements;
};
