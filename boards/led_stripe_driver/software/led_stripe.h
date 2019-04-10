
#ifndef LED_STRIPE_H
#define LED_STRIPE_H

//#include <unistd.h> // usleep
//#include <util/delay.h>

#include <stdint.h>

#include "light_ws2812_config.h"
#include "light_ws2812.h"


// 16MHz Clock
#define F_CPU 16000000UL
 
// Calculate the value needed for 
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000UL) / 8UL) 

// PF0 = Analog 0
// PC0 = Diginal 37
// src: https://www.arduino.cc/en/uploads/Hacking/PinMap2560big.png

typedef uint8_t Command[11];

/*
 * IIIAAACC
 *       11 0x03
 *    11100 0x1c
 * 11100000 0xe0
 */

typedef enum {
	COLOR_MOD = 0, // light up every k'th LED in this color
	COLOR_GRADIENT = 1, // gradient (color0 -> color1)
	COLOR_BITS = 2,
	COLOR_MASK = 0x03 //(1 << (COLOR_BITS + 1)) - 1

} ColorMode;

typedef enum {
	ANIM_ROTATE = 0x00,
	ANIM_FADEOUT = 0x04,
	ANIM_FLASH = 0x08,
	ANIM_BITS = 3,
	ANIM_MASK = 0x1c //((1 << (ANIM_BITS + 1)) - 1) << COLOR_BITS
} AnimationMode;

enum {
	ID_MASK = 0xe0
	//ID_MASK = (0xff << (ANIM_BITS + COLOR_BITS)) & 0xff
};

inline ColorMode     color_mode(const Command c) { return c[0] & COLOR_MASK; }
inline AnimationMode anim_mode(const Command c)  { return c[0] & ANIM_MASK; }
inline int           id(const Command c)         { return c[0] & ID_MASK; }

// Color

inline int  mod  (const Command c) { return c[ 1]; }

inline int  r0   (const Command c) { return c[ 2]; }
inline int  g0   (const Command c) { return c[ 3]; }
inline int  b0   (const Command c) { return c[ 4]; }

inline int  r1   (const Command c) { return c[ 5]; }
inline int  g1   (const Command c) { return c[ 6]; }
inline int  b1   (const Command c) { return c[ 7]; }

// Animation

inline int  dir  (const Command c) { return c[ 8]; }
inline int  dt   (const Command c) { return c[ 9]; }
inline int  count(const Command c) { return c[10]; }


int main(void);
void setup(void);
void setup_spi(void);
void start_execute(void);
void execute(void);
void clear_leds(void);
void xfer_spi(void);
void swap_buffers(void);
uint8_t* active_buffer(void);
uint8_t* inactive_buffer(void);
void load(Command);
void mirror_leds(void);

#endif

