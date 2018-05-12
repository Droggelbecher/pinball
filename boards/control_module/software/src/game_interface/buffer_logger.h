#ifndef buffer_logger_h_INCLUDED
#define buffer_logger_h_INCLUDED

#include <sstream>

#include "ring_buffer.h"

namespace pinball {

template<int N_>
class BufferLogger {
		using Buffer = RingBuffer<std::string, N_>;

	public:
		template<typename T>
		BufferLogger& operator<<(const T& v) {
			line_buffer_ << v;
			std::string line;

			if(!std::getline(line_buffer_, line)) {
				return *this;
			}
			if(line_buffer_.eof()) {
				// Line is not ready yet, put it back in
				line_buffer_.clear();
				line_buffer_ << line;
			}
			else {
				line_buffer_.clear();
				lines_.push_back(line);
			}
			return *this;
		}

		typename Buffer::iterator begin() {
			return lines_.begin();
		}

		typename Buffer::iterator end() {
			return lines_.end();
		}


	private:
		RingBuffer<std::string, N_> lines_;
		std::stringstream line_buffer_;
};

} // ns pinball

#endif // buffer_logger_h_INCLUDED

