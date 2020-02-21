
class Player {
	public:
		size_t score = 0;
		size_t balls = 3;

		void reset() {
			_multiplier = 1;
		}

		void increase_multiplier(int n) {
			_multiplier *= n;
			if(_multiplier > 10) {
				_multiplier = 10;
			}
			if(_multiplier < 1) {
				_multiplier = 1;
			}
		}

		@property
		int multiplier() {
			return _multiplier;
		}

	private:
		int _multiplier = 1;
}

