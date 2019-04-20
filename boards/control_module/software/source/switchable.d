
mixin template Switchable() {
	void off() {
		this.enabled = false;
	}

	void on() {
		this.enabled = true;
	}

	void toggle() {
		this.enabled = !this.enabled;
	}

	private {
		bool enabled = false;
	}
}

