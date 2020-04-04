
mixin template Switchable() {
	void off() {
		this._enabled = false;
	}

	void on(bool enabled=true) {
		this._enabled = enabled;
	}

	void toggle() {
		this._enabled = !this._enabled;
	}

	bool enabled() {
		return _enabled;
	}

	private {
		bool _enabled = false;
	}
}

