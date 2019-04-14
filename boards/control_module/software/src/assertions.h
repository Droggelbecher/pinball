#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include "coordinate.h"

namespace pinball {
	template<typename Ca, typename Cb>
	void assert_blittable(const Ca& ca, Cb& cb, Coordinate<> from, Coordinate<> to, Coordinate<> tgt) {
		assert(0 <= from.row() && from.row() <= ca.size().row());
		assert(0 <= from.column() && from.column() <= ca.size().column());
		assert(0 <= to.row() && to.row() <= ca.size().row());
		assert(0 <= to.column() && to.column() <= ca.size().column());

		assert(0 <= tgt.row() && tgt.row() <= cb.size().row());
		assert(0 <= tgt.column() && tgt.column() <= cb.size().column());
		Coordinate<> tgt_end = tgt + to - from;
		assert(0 <= tgt_end.row() && tgt_end.row() <= cb.size().row());
		assert(0 <= tgt_end.column() && tgt_end.column() <= cb.size().column());
	}
}

#endif // ASSERTIONS_H

