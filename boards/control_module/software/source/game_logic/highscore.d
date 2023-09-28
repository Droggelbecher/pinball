
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
	this(Interface iface) {
		this.scores[] = tuple("Luke", 0);
		this.iface = iface;
		try_load();
	}

	Tuple!(string, int)[] get() {
		return scores;
	}

	void add(string name, int score) {
		// Overwrite the extra low score entry
		this.scores[$-1] = tuple(name, score);

		// put it in place
		fix_scores();

		try_save();
	}
}


// TODO: Make this only keep the top N items, possibly turn this into a class
/*
Tuple!(string, int)[] top(int n) {
	auto file = File(FILENAME, "r");

	Tuple!(string, int)[] r;

	foreach(record;csvReader!(Tuple!(string, int))(file.byLine.joiner("\n"))) {
		r ~= record;
	}
	// Sort descending by score
	r.sort!("a[1] > b[1]");
	// TODO: if len < n, fill up with dummies
	return r[0 .. n];
}

void add_entry(string name, ulong score) {
	append(FILENAME, format!"%s,%d\n"(name, score));
}
*/
