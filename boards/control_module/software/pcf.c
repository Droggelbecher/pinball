
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h> /* memset */

#include "pcf.h"


uint16_t pcf_read_lsbint16(FILE *fp) {
	uint16_t r = 0;
	int ch;

	ch = fgetc(fp); if(ch != EOF) { r = (uint16_t)ch; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint16_t)ch << 8; }
	return r;
}

uint16_t pcf_read_msbint16(FILE *fp) {
	uint16_t r = 0;
	int ch;

	ch = fgetc(fp); if(ch != EOF) { r = (uint16_t)ch << 8; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint16_t)ch; }
	return r;
}

uint32_t pcf_read_lsbint32(FILE *fp) {
	uint32_t r = 0;
	int ch;

	ch = fgetc(fp); if(ch != EOF) { r = (uint32_t)ch; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch << 8; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch << 16; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch << 24; }
	return r;
}

uint32_t pcf_read_msbint32(FILE *fp) {
	uint32_t r = 0;
	int ch;

	ch = fgetc(fp); if(ch != EOF) { r = (uint32_t)ch << 24; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch << 16; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch <<  8; }
	ch = fgetc(fp); if(ch != EOF) { r |= (uint32_t)ch; }
	return r;
}

uint32_t pcf_read_int32(FILE *fp, unsigned char format) {
	if(format & PCF_BYTE_MASK) {
		return pcf_read_msbint32(fp);
	}
	else {
		return pcf_read_lsbint32(fp);
	}
}

uint16_t pcf_read_int16(FILE *fp, unsigned char format) {
	if(format & PCF_BYTE_MASK) {
		return pcf_read_msbint16(fp);
	}
	else {
		return pcf_read_lsbint16(fp);
	}
}

void pcf_read_toc_entry(FILE *fp, struct pcf_toc_entry *r) {
	r->type = pcf_read_lsbint32(fp);
	r->format = pcf_read_lsbint32(fp);
	r->size = pcf_read_lsbint32(fp);
	r->offset = pcf_read_lsbint32(fp);
}

void pcf_destroy_toc_entry(struct pcf_toc_entry *e) {
}

void pcf_read_encoding(FILE *fp, struct pcf_encoding *r) {
	r->format = pcf_read_lsbint32(fp);
	r->min_char_or_byte2 = pcf_read_int16(fp, r->format);
	r->max_char_or_byte2 = pcf_read_int16(fp, r->format);
	r->min_byte1 = pcf_read_int16(fp, r->format);
	r->max_byte1 = pcf_read_int16(fp, r->format);
	r->default_char = pcf_read_int16(fp, r->format);

	uint32_t sz = 2 * (r->max_char_or_byte2 - r->min_char_or_byte2 + 1) * (r->max_byte1 - r->min_byte1 + 1);
	r->glyphindeces = malloc(sz);
	fread(r->glyphindeces, sz, 1, fp);
	if(!!(r->format & PCF_BYTE_MASK) == is_msb_first()) {
		printf("swapping\n");
		swap_bytes((uint8_t*)r->glyphindeces, sz);
	}
}

void pcf_destroy_encoding(struct pcf_encoding *e) {
	free(e->glyphindeces);
	e->glyphindeces = 0;
}

void pcf_read_bitmap(FILE *fp, struct pcf_bitmap *r) {
	r->format = pcf_read_lsbint32(fp);
	r->glyph_count = pcf_read_int32(fp, r->format);
	r->offsets = malloc(sizeof(uint32_t) * r->glyph_count);
	int j;
	for(j = 0; j < r->glyph_count; j++) {
		r->offsets[j] = pcf_read_int32(fp, r->format);
	}
	for(j = 0; j < 4; j++) {
		r->bitmap_sizes[j] = pcf_read_int32(fp, r->format);
	}
	uint32_t sz = r->bitmap_sizes[r->format & PCF_GLYPH_PAD_MASK];
	r->bitmap_data = malloc(sz);
	fread(r->bitmap_data, sz, 1, fp);
}

void pcf_destroy_bitmap(struct pcf_bitmap *r) {
	free(r->offsets); r->offsets = 0;
}

void pcf_read_font(char *filename, struct pcf_font *font) {

	struct pcf_encoding encoding;
	struct pcf_bitmap bitmap;
	int has_encoding = 0;
	int has_bitmap = 0;

	FILE *fp = fopen(filename, "r");

	if(!fp) {
		perror("couldnt open PCF file.\n");
		exit(1);
	}

	int ch;
	ch = fgetc(fp); if(ch != EOF) { assert(ch == '\1'); }
	ch = fgetc(fp); if(ch != EOF) { assert(ch == 'f'); }
	ch = fgetc(fp); if(ch != EOF) { assert(ch == 'c'); }
	ch = fgetc(fp); if(ch != EOF) { assert(ch == 'p'); }

	uint32_t table_count = pcf_read_lsbint32(fp);
	int i;
	for(i = 0; i < table_count; i++) {
		struct pcf_toc_entry entry;
		pcf_read_toc_entry(fp, &entry);
		/*printf("i=%d entry.type=%d\n", i, entry.type);*/

		long pos = ftell(fp);
		
		fseek(fp, entry.offset, SEEK_SET);

		switch(entry.type) {
			case PCF_BDF_ENCODINGS:
				pcf_read_encoding(fp, &encoding);
				has_encoding = 1;
				break;

			case PCF_BITMAPS:
				pcf_read_bitmap(fp, &bitmap);
				has_bitmap = 1;
				break;

		}

		fseek(fp, pos, SEEK_SET);
	}

	fclose(fp);

	assert(has_encoding);
	assert(has_bitmap);

	memset(font->offsets, 0xff, sizeof(font->offsets));

	// Bitmap data

	/*int sz = bitmap.format & PCF_GLYPH_PAD_MASK;*/
	font->bitmap_size = bitmap.bitmap_sizes[1];
	font->bitmap_data = malloc(font->bitmap_size);


	int target_offset = 0;
	const int bytes_per_row = 1 << (bitmap.format & PCF_GLYPH_PAD_MASK);
	const float size_factor = 2.0 / bytes_per_row;
	/*printf("size factor %f\n", size_factor);*/

	int glyph;
	int offset_idx = 0;

	for(glyph = 0; glyph < bitmap.glyph_count; glyph++) {
		int offset_start = bitmap.offsets[glyph];
		int offset_end = bitmap.offsets[glyph + 1]; // TODO

		/*printf("src=%d target offset= %d\n", offset_start, target_offset);*/
		
		transform_bytes(bitmap.bitmap_data + offset_start, bitmap.bitmap_data + offset_end, font->bitmap_data + target_offset, bitmap.format);
		target_offset += (offset_end - offset_start) * size_factor;
	}

	int j;
	for(j = encoding.min_char_or_byte2; j <= encoding.max_char_or_byte2; j++) {
		/*printf("j=%d enc.gi[j-..]=%d\n", j,encoding.glyphindeces[j-encoding.min_char_or_byte2]);*/
		uint16_t glyph_idx = encoding.glyphindeces[j-encoding.min_char_or_byte2];
		uint16_t offs = 0xffff;
		if(glyph_idx != 0xffff) {
			offs = bitmap.offsets[glyph_idx];
		}
		if(offs != 0xffff) {
			offs *= size_factor;
		}

		font->offsets[j] = offs;
		/*printf("offsets[%d] = (%d) %d\n", j, encoding.glyphindeces[j-encoding.min_char_or_byte2], font->offsets[j]);*/
	}

}

void transform_bytes(uint8_t *source_start, uint8_t *source_end, uint8_t *target_start, uint32_t format) {
	const int reverse_bits = !!(format & PCF_BIT_MASK);
	const int reverse_bytes = !!(format & PCF_BYTE_MASK);
	const int bytes_per_row1 = format & PCF_GLYPH_PAD_MASK;
	const int bytes_per_row = 1 << (format & PCF_GLYPH_PAD_MASK);

	//                        1         2        4
	int select_bytes[][2] = { { 0, 0 }, { 0, 1}, { 0, 1 }, { 1, 2 } };
	int byte0;
	int byte1;

	if(reverse_bytes) {
		byte0 = select_bytes[bytes_per_row1][1];
		byte1 = select_bytes[bytes_per_row1][0];
	}
	else {
		byte0 = select_bytes[bytes_per_row1][0];
		byte1 = select_bytes[bytes_per_row1][1];
	}

	for( ; source_start < source_end; source_start += bytes_per_row) {
		if(reverse_bits) {
			*target_start++ = reverse_byte(source_start[byte0]);
			*target_start++ = reverse_byte(source_start[byte1]);
		}
		else {
			*target_start++ = source_start[byte0];
			*target_start++ = source_start[byte1];
		}
	}
}


void pcf_destroy_font(struct pcf_font *font) {
	// TODO
}

uint8_t reverse_byte(uint8_t b) {
	// source: https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
	return (uint8_t)((b * 0x0202020202ULL & 0x010884422010ULL) % 1023);
}

void swap_bytes(uint8_t *p, size_t sz) {
	int i = 0;
	for( ; i < sz; i+=2) {
		p[i] ^= p[i+1]; // p'[i] = p[i] ^ p[i+1]
		p[i+1] ^= p[i];  // p1[i+1= p[i+1] ^ p[i] ^ p[i+1] = p[i]
		p[i] ^= p[i+1];
	}
}



int is_big_endian(void) {
	union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1; 
}

void pcf_print_font(struct pcf_font *font) {
	int i = 0;
	for(i = 0; i < 256; i++) {
		printf("-- %3d\n", i);
		if(font->offsets[i] == 0xffff) { continue; }
		int offs = font->offsets[i];
		for( ; offs < font->offsets[i+1]; offs += 2) {
			int j = 0;
			for(j = 0; j < 8; j++) {
				putchar(font->bitmap_data[offs] & (1 << j) ? '#' : ' ');
			}
			for(j = 0; j < 8; j++) {
				putchar(font->bitmap_data[offs + 1] & (1 << j) ? '#' : ' ');
			}
			putchar('\n');
		} // offs
	} // i
}

void pcf_render_string(struct pcf_font *font, char *s, uint8_t *buffer, uint16_t buffer_cols, uint16_t buffer_rows, uint16_t start_row, uint16_t start_col, uint8_t color) {

	uint16_t padding_left = 8;
	uint16_t width = 8;

	for( ; *s != '\0'; s++) {
		if(font->offsets[c] == 0xffff) {
			continue;
		}

		int row = 0;
		for( ; font_offsets[c] + 2 * row < font_offsets[c + 1] && start_row + row < buffer_rows; row++) {

			int col = 0; // col is relative column for font
			for(col = padding_left; col < width && start_col + col < 8 && col < buffer_cols; col++) {
				if(font->bitmap_data[font_offsets[c] + 2 * row] & (1 << col)) {
					buffer[(start_row + row) * buffer_cols + start_col + col] = color;
				}
			}
			for(col = 8; col < width && col < 16 && start_col + col < buffer_cols; col++) {
				if(font->bitmap_data[font_offsets[c] + 2 * row + 1] & (1 << (col-8))) {
					buffer[(start_row + row) * buffer_cols + start_col + col] = color;
				}
			}

			start_col += width;
		}
	}
}


void main(int argc, char **argv) {
	printf("reading %s\n", argv[1]);

	struct pcf_font gohufont;

	pcf_read_font(argv[1], &gohufont);
	/*pcf_print_font(&gohufont);*/
}
