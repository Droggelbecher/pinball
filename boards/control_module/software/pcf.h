
#ifndef PCF_H
#define PCF_H

#include <stdint.h>

// Sources:
// https://fontforge.github.io/pcf-format.html


#define PCF_PROPERTIES            (1<<0)
#define PCF_ACCELERATORS          (1<<1)
#define PCF_METRICS               (1<<2)
#define PCF_BITMAPS               (1<<3)
#define PCF_INK_METRICS           (1<<4)
#define PCF_BDF_ENCODINGS         (1<<5)
#define PCF_SWIDTHS               (1<<6)
#define PCF_GLYPH_NAMES           (1<<7)
#define PCF_BDF_ACCELERATORS      (1<<8)

#define PCF_DEFAULT_FORMAT        0x00000000
#define PCF_INKBOUNDS             0x00000200
#define PCF_ACCEL_W_INKBOUNDS     0x00000100
#define PCF_COMPRESSED_METRICS    0x00000100

#define PCF_GLYPH_PAD_MASK        (3<<0)        /* See the bitmap table for explanation */
#define PCF_BYTE_MASK             (1<<2)        /* If set then Most Sig Byte First */
#define PCF_BIT_MASK              (1<<3)        /* If set then Most Sig Bit First */
#define PCF_SCAN_UNIT_MASK        (3<<4)        /* See the bitmap table for explanation */

#define PCF_NOGLYPH               0xffff

struct pcf_toc_entry {
	uint32_t type;
	uint32_t format;
	uint32_t size;
	uint32_t offset;
};

struct pcf_encoding {
	uint32_t format;
	uint16_t min_char_or_byte2;
	uint16_t max_char_or_byte2;
	uint16_t min_byte1;
	uint16_t max_byte1;
	uint16_t default_char;
	uint16_t *glyphindeces;
	//[(max_char_or_byte2-min_char_or_byte2+1)*(max_byte1-min_byte1+1)];
};

struct pcf_font {
	uint16_t offsets[256];
	uint16_t lengths[256];

	uint16_t bitmap_size;
	uint8_t *bitmap_data;
};

struct pcf_bitmap {
	uint32_t format;
	uint32_t glyph_count;
	uint32_t *offsets;
	uint32_t bitmap_sizes[4];
	uint8_t *bitmap_data;
};

uint16_t pcf_read_lsbint16(FILE *);
uint16_t pcf_read_msbint16(FILE *);
uint16_t pcf_read_int16(FILE *, unsigned char);
uint32_t pcf_read_lsbint32(FILE *);
uint32_t pcf_read_msbint32(FILE *);
uint32_t pcf_read_int32(FILE *, unsigned char);

void pcf_read_toc_entry(FILE *, struct pcf_toc_entry *);
void pcf_destroy_toc_entry(struct pcf_toc_entry *);

void pcf_read_encoding(FILE *, struct pcf_encoding *);
void pcf_destroy_encoding(struct pcf_encoding *);

void pcf_read_bitmap(FILE *, struct pcf_bitmap *);
void pcf_destroy_bitmap(struct pcf_bitmap *);

void pcf_read_font(char *, struct pcf_font *);

// TODO: Move these somewhere else as they are not PCF specific

void transform_bytes(uint8_t *source_start, uint8_t *source_end, uint8_t *target_start, uint32_t format);
uint8_t reverse_byte(uint8_t b);
void swap_bytes(uint8_t *p, size_t sz);

//void pcf_render_string(struct pcf_font *font, char *s, uint8_t *buffer, uint16_t buffer_cols, uint16_t buffer_rows, uint16_t start_row, uint16_t start_col, uint8_t color);

/**
 * @return 1 iff at least one pixel of the font was rendered.
 */
uint8_t pcf_render_char(struct pcf_font *font, char c, uint16_t row, uint16_t col, uint8_t color);

/**
 * @return 1 iff at least one pixel of the font was rendered.
 */
uint8_t pcf_render_string(struct pcf_font *font, char *s,  uint16_t start_row, uint16_t start_col, uint8_t color);

static uint32_t _is_big_endian;
#define is_msb_first() ( (*(char*)&_is_big_endian) == 0 )

#endif // PCF_H

