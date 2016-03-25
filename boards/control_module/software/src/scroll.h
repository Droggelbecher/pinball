
#ifndef SCROLL_H
#define SCROLL_H

struct scroll_state {
	/// unit: pixel/s
	int16_t speed_columns;
	/// unit: pixel/s
	int16_t speed_rows;

	int16_t start_column;
	int16_t start_row;
	int16_t current_column;
	int16_t current_row;

	unsigned long next_column_frame_;
	unsigned long next_row_frame_;
};

void scroll_compute(struct scroll_state *);
void scroll_reset(struct scroll_state *);

#endif // SCROLL_H

