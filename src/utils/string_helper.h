#include <cctype>
#include <string>

class StringHelper {
public:
	static inline size_t case_insensitive_find(const std::string& str, const std::string& substr) {
    	if (substr.empty()) return 0;     	
    	auto to_lower = [](unsigned char c) { return std::tolower(c); };
    	
    	for (size_t i = 0; i + substr.size() <= str.size(); ++i) {
        	bool match = true;
        	for (size_t j = 0; j < substr.size(); ++j) {
        	    if (to_lower(str[i + j]) != to_lower(substr[j])) {
        	        match = false;
        	        break;
        	    }
        	}
        	if (match) {
        	    return i; 
       		}
   		}
    	return std::string::npos;
	} 
};
