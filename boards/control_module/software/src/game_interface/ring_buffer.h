#ifndef ring_buffer_h_INCLUDED
#define ring_buffer_h_INCLUDED

#include <array>

template<typename T, int N_>
class RingBuffer {
	public:

		class iterator {
			public:
				iterator(std::array<T, N_>& array, int idx)
					: array_(array), idx_(idx)
					{ }

				iterator& operator++() {
					idx_ = (idx_ + 1) % N_;
					return *this;
				}

				T& operator*() {
					return array_[idx_];
				}

				bool operator==(const iterator& other) const {
					return idx_ == other.idx_;
				}

				bool operator!=(const iterator& other) const {
					return idx_ != other.idx_;
				}

			private:
				std::array<T, N_>& array_;
				int idx_;
		};


		void push_back(const T& v) {
			array_[end_] = v;
			end_ = (end_ + 1) % N_;
			if(begin_ == end_) {
				begin_ = (begin_ + 1) % N_;
			}
		}

		iterator begin() {
			return iterator(array_, begin_);
		}

		iterator end() {
			return iterator(array_, end_);
		}

	private:
		std::array<T, N_> array_;
		int begin_ = 0;
		int end_ = 0;
};

#endif // ring_buffer_h_INCLUDED

