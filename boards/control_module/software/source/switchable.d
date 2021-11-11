
mixin template Switchable() {
	void off() {
		this.on(false);
	}

	void on(bool enabled=true) {
		this._enabled = enabled;
	}

	void toggle() {
		this.on(!this.enabled());
	}

	bool enabled() {
		return _enabled;
	}

	private {
		bool _enabled = false;
	}
}

