
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#define BUTTONS_FLIPPER_LEFT_IDX   2
#define BUTTONS_FLIPPER_RIGHT_IDX  5

uint8_t buttons_state[9];

void buttons_read(void);
uint8_t buttons_get(uint8_t);

#endif // BUTTONS_H

