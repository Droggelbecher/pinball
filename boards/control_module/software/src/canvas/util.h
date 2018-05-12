#ifndef util_h
#define util_h

#include "coordinate.h"

namespace pinball {

typedef enum {
	COLUMN_FIRST,
	ROW_FIRST
} DataOrder;


/**
 * Copy the rectangle ($start, $end) from $a to $b at location $target.
 */
template<typename CanvasA, typename CanvasB>
void blit(const CanvasA& a, CanvasB& b, Coordinate<> start, Coordinate<> end, Coordinate<> target) {

	static_assert(CanvasA::data_order == CanvasB::data_order, "Can only blit with equal data orders");

	static_assert(CanvasA::data_order == COLUMN_FIRST, "Blit only implemented for COLUMN_FIRST currently");

	int n_a = a.size().column();
	int n_b = b.size().column();
	int len = end.column() - start.column();

	const uint8_t *buf_a = a.buffer() + start.column();
	uint8_t *buf_b = b.buffer() + target.column();

	// copy row by row
	for(int row = start.row(); row < end.row(); ++row) {
		memcpy(
			buf_b + row * n_a,
			buf_a + row * n_b,
			len
			);
	}
}

} // ns pinball

#endif // util_h

