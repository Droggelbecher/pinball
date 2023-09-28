
/**
  Track player-specific game state for a single player such as score and number of balls.
  */
class Player {
	public:
		size_t score = 0;
		enum MAX_MULTIPLIER = 10;
		enum MAX_BALLS = 99;

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
			if(_balls > MAX_BALLS) {
				_balls = MAX_BALLS;
			}
		}

		// True iff player is still in the game
		bool alive() {
			return _balls > 0;
		}

		void reset() {
			_multiplier = 1;
		}

		/**
		  Increase multiplier by factor `n`.
		  */
		void increase_multiplier(int n) {
			_multiplier *= n;
			if(_multiplier > MAX_MULTIPLIER) {
				_multiplier = MAX_MULTIPLIER;
			}
			if(_multiplier < 1) {
				_multiplier = 1;
			}
		}

		/**
		  All future scorings get multiplied with this factor.
		  */
		@property
		int multiplier() {
			return _multiplier;
		}

	private:
		int _multiplier = 1;
		// Set to 1 for debugging
		int _balls = 1;
}

