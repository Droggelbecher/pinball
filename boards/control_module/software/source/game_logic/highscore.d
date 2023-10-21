
import std.algorithm;
import std.array;
import std.csv;
import std.stdio;
import std.format;
import std.typecons;
import std.conv;
import std.file;
import std.algorithm.sorting;
import core.exception;

auto FILENAME = "scores.csv";
const int N_SCORES = 5;

class Highscore(Interface) {
private:
	// We keep a highscore of N_SCORES plus one extra "low score" entry
	// for easier insertion of new scores
	// Methods ensure this always stays sorted descending by score
	Tuple!(string, int)[N_SCORES + 1] scores;
	Interface iface;

	void try_load() {
		try {
			auto file = File(FILENAME, "r");
			auto csv = file.byLine.joiner("\n").csvReader!(Tuple!(string, int));
			this.scores[0 .. N_SCORES] = csv.array;
		}
		catch(Exception e) {
			this.iface.logger.logf("Could not read high scores:", e.msg);
		}
		catch(RangeError e) {
			this.iface.logger.logf("Could not read high scores:", e.msg);
		}

		fix_scores();
	}

	void try_save() {
		try {
			string temp_filename = FILENAME ~ ".tmp";
			auto file = File(temp_filename, "w");
			foreach(score; scores[0 .. N_SCORES]) {
				file.write(score[0] ~ "," ~ score[1].to!string ~ "\n");
			}
			file.close();

			// Move temporary file in place.
			// This way file writing is more "atomic" and chances
			// of corrupting the file due to an unfortunately-timed power-down are hopefully lower.
			std.file.rename(temp_filename, FILENAME);
		}
		catch(Exception e) {
			this.iface.logger.logf("Could not write high scores:", e.msg);
		}
		catch(RangeError e) {
			this.iface.logger.logf("Could not write high scores:", e.msg);
		}
	}

	// Fix scores in-place to be sorted 
	void fix_scores() {
		// Sort descending by score.
		// Not strictly necessary for every operation such as 'add' (a single insertion sort step
		// would be enough), but "n" is incredibly low here so no point in arguing about asymptotic
		// complexity here..
		sort!("a[1] > b[1]")(this.scores[]);
	}

public:
	int[] get_highscore_indices(int[] scores) {

		Tuple!(int, int)[] hiscores;
		hiscores.reserve(this.scores.length + scores.length);
		hiscores.length = this.scores.length + scores.length;

		for (int i = 0; i < this.scores.length; i++) {
			// Previous high score
			hiscores[i] = tuple(this.scores[i][1], -1);
		}
		for (int i = 0; i < scores.length; i++) {
			// index of new score
			hiscores[i + this.scores.length] = tuple(scores[i], i);
		}

		sort!("a[0] > b[0]")(hiscores[]);

		int[] r;
		for (int i = 0; i < N_SCORES; i++) {
			if (hiscores[i][1] != -1) {
				r ~= hiscores[i][1];
			}
		}

		return r;
	}

	this(Interface iface) {
		this.scores[] = tuple("Luke", 0);
		this.iface = iface;
		try_load();
	}

	Tuple!(string, int)[] get() {
		return scores[0..$-1];
	}

	void add(string name, int score) {
		// Overwrite the extra low score entry
		this.scores[$-1] = tuple(name, score);

		// put it in place
		fix_scores();

		try_save();
	}
}


