
class Player {
	public:
		size_t score = 0;

		@property
		int balls() {
			return _balls;
		}

		@property
		void balls(int b) {
			_balls = b;
			if(_balls < 0) {
				_balls = 0;
			}
			if(_balls > 99) {
				_balls = 99;
			}
		}

		bool alive() {
			return _balls > 0;
		}

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
		// 1 for debugging
		int _balls = 1;
}

