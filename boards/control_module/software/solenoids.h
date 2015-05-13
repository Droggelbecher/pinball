
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include <stdint.h>

// These should match the ..._IDX values in ../../solenoid_driver/solenoid_driver.h
#define SOLENOIDS_FLIPPER_LEFT_IDX 0
#define SOLENOIDS_FLIPPER_RIGHT_IDX 1

uint8_t solenoids_state[2];


void solenoids_write(void);
void solenoids_set(uint8_t, uint8_t);

#endif // SOLENOIDS_H

