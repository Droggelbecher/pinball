
import std.algorithm;
import std.array;
import std.csv;
import std.stdio;
import std.format;
import std.typecons;
import std.file: append;

auto FILENAME = "scores.csv";

// TODO: Make this only keep the top N items, possibly turn this into a class

Tuple!(string, int)[] top(int n) {
	auto file = File(FILENAME, "r");

	Tuple!(string, int)[] r;

	foreach(record;csvReader!(Tuple!(string, int))(file.byLine.joiner("\n"))) {
		r ~= record;
	}
	// Sort descending by score
	r.sort!("a[1] > b[1]");
	return r[0 .. n];
}

void add_entry(string name, ulong score) {
	append(FILENAME, format!"%s,%d\n"(name, score));
}
