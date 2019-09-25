
mixin template Switchable() {
	void off() {
		this._enabled = false;
	}

	void on() {
		this._enabled = true;
	}

	void toggle() {
		this._enabled = !this._enabled;
	}

	bool enabled() {
		return _enabled && enabled_condition();
	}

	bool enabled_condition() {
		return true;
	}

	private {
		bool _enabled = false;
	}
}

