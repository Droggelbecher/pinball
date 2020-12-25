
import std.algorithm;
import std.array;
import std.csv;
import std.stdio;
import std.typecons;

Tuple!(string, int)[] read() {
	auto file = File("scores.csv", "r");

	Tuple!(string, int)[] r;

	foreach(record;csvReader!(Tuple!(string, int))(file.byLine.joiner("\n"))) {
		r ~= record;
	}
	// Sort descending by score
	r.sort!("a[1] > b[1]");
	return r[0..5];
}

