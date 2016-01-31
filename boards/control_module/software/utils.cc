
#include "utils.h"

std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& v) {
	os << "[";
	for(auto it = begin(v); it != end(v); ++it) {
		os << (int)*it << ",";
	}
	os << "]";
	return os;
}

