
import std.bitmanip;
import std.datetime;
import std.stdio;
import std.typecons;
import utils;

/**
 * Decorate a given SensorActutator in the following way:
 * - set()ing is intercepted and sets the state of the decorator
 * - get()ing returns decorated XOR decorator iff enabled, else it shows state of decorated
 * 
 * Thus, enable_xor gives a masking behavior, whereas disabled will just
 * throw away all set() calls and pass through get().
 */
class SensorActuatorOverride(SensorActuator) {
	alias SensorActuator.Index Index;

	enum {
		DATA_BITS = Index.MAX,
		DATA_BYTES = (DATA_BITS +7) / 8,
		DATA_WORDS = (DATA_BITS + 8*size_t.sizeof - 1) / (8*size_t.sizeof),
	}

	this(SensorActuator decorated, Flag!"mask_get" mask_get, Flag!"mute_set" mute_set) {
		this.mask_get = mask_get;
		this.mute_set = mute_set;
		this.decorated = decorated;
		this.state_data[] = cast(size_t)0;
		this.mask = BitArray(state_data, Index.MAX);
	}

	@nogc
	bool opIndex(Index idx) {
		if(mask_get) {
			return mask[cast(uint)idx] != decorated[idx];
		}
		else {
			return decorated[idx];
		}
	}

	@nogc
	bool opIndexAssign(bool v, Index idx) {
		if(!mute_set) {
			decorated[idx] = v;
		}
		return v;
	}

	private {
		void print_state(Index idx) {
			writeln("idx=%s dec=%s mask=%s".format(idx, decorated, mask));
		}
	}

	BitArray mask;
	bool mask_get;
	bool mute_set;
	SensorActuator decorated;

	private:
		size_t[DATA_WORDS + 1] state_data;
}

