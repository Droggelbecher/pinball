
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// Sources:
// https://fontforge.github.io/pcf-format.html


#define PCF_PROPERTIES		    (1<<0)
#define PCF_ACCELERATORS	    (1<<1)
#define PCF_METRICS		    (1<<2)
#define PCF_BITMAPS		    (1<<3)
#define PCF_INK_METRICS		    (1<<4)
#define	PCF_BDF_ENCODINGS	    (1<<5)
#define PCF_SWIDTHS		    (1<<6)
#define PCF_GLYPH_NAMES		    (1<<7)
#define PCF_BDF_ACCELERATORS	    (1<<8)

#define PCF_DEFAULT_FORMAT	0x00000000
#define PCF_INKBOUNDS		0x00000200
#define PCF_ACCEL_W_INKBOUNDS	0x00000100
#define PCF_COMPRESSED_METRICS	0x00000100

#define PCF_GLYPH_PAD_MASK	(3<<0)		/* See the bitmap table for explanation */
#define PCF_BYTE_MASK		(1<<2)		/* If set then Most Sig Byte First */
#define PCF_BIT_MASK		(1<<3)		/* If set then Most Sig Bit First */
#define PCF_SCAN_UNIT_MASK	(3<<4)		/* See the bitmap table for explanation */

struct toc_entry {
	uint32_t type;
	uint32_t format;
	uint32_t size;
	uint32_t offset;
};

struct encoding {
	uint32_t format;
	uint16_t min_char_or_byte2;
	uint16_t max_char_or_byte2;
	uint16_t min_byte1;
	uint16_t max_byte1;
	uint16_t default_char;
	uint16_t *glyphindeces;
	//[(max_char_or_byte2-min_char_or_byte2+1)*(max_byte1-min_byte1+1)];
};

// TODO:
// Ziel: PCF file in font struct laden, dann
// funktionen haben wie
// display_render_text(font*, char*, int x, int y);
struct font_t; {
	uint16_t offsets[256];

	uint16_t bitmap_size;
	uint16_t *bitmap_data;
};

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

void pcf_read_toc_entry(FILE *fp, struct toc_entry *r) {
	r->type = pcf_read_lsbint32(fp);
	r->format = pcf_read_lsbint32(fp);
	r->size = pcf_read_lsbint32(fp);
	r->offset = pcf_read_lsbint32(fp);
}

void destroy_toc_entry(struct toc_entry *e) {
}

void pcf_read_encoding(FILE *fp, struct encoding *r) {
	r->format = pcf_read_lsbint32(fp);
	r->min_char_or_byte2 = pcf_read_int16(fp, r->format);
	r->max_char_or_byte2 = pcf_read_int16(fp, r->format);
	r->min_byte1 = pcf_read_int16(fp, r->format);
	r->max_byte1 = pcf_read_int16(fp, r->format);
	r->default_char = pcf_read_int16(fp, r->format);

	uint32_t sz = 2 * (r->max_char_or_byte2 - r->min_char_or_byte2 + 1) * (r->max_byte1 - r->min_byte1 + 1);
	r->glyphindeces = malloc(sz);
	fread(r->glyphindeces, sz, 1, fp);
}

void destroy_encoding(struct encoding *e) {
	free(r->glyphindeces);
	r->glyphindeces = 0;
}

void pcf_read_bitmap(FILE *fp, struct bitmap *r) {
	r->format = pcf_read_lsbint32(fp);
	r->glyph_count = pcf_read_int32(fp, r->format);
	r->offsets = malloc(sizeof(uint32_t) * r->glyph_count);
	int j;
	for(j = 0; j < glyph_count; j++) {
		r->offsets[j] = pcf_read_int32(fp, r->format);
	}
	for(j = 0; j < 4; j++) {
		r->bitmap_sizes[j] = pcf_read_int32(fp, r->format);
	}
	uint32_t sz = r->bitmap_sizes[r->format & GLYPH_PAD_MASK];
	r->bitmap_data = malloc(sz);
	fread(r->bitmap_data, sz, 1, fp);
}

void destroy_bitmap(struct bitmap *r) {
	free(r->offsets); r->offsets = 0;
}

void pcf_read_font(char *filename, font_t *font) {
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
		struct toc_entry entry;
		pcf_read_toc_entry(fp, &entry);
		printf("i=%d entry.type=%d\n", i, entry.type);
		
		fseek(fp, entry.offset, SEEK_SET);

		if(entry.type == PCF_BDF_ENCODINGS) {
			uint32_t format = pcf_read_lsbint32(fp);
			// TODO
		}
			

		else if(entry.type == PCF_BITMAPS) {
			if(font->bitmap_data != 0) {
				perror("we already had a bitmap table. I'm confused.");
				exit(1);
			}

			uint32_t format = pcf_read_lsbint32(fp);
			uint32_t glyph_count = pcf_read_int32(fp, format);

			printf("glyph count: %lu\n", glyph_count);

			int j;

			uint32_t *offsets = malloc(sizeof(uint32_t) * glyph_count);
			for(j = 0; j < glyph_count; j++) {
				offsets[j] = pcf_read_int32(fp, format);
				//printf("offsets[%d] = %lu\n", j, offsets[j]);
			}

			uint32_t bitmap_sizes[4];
			for(j = 0; j < 4; j++) {
				bitmap_sizes[j] = pcf_read_int32(fp, format);
				//printf("bitmap_sizes[%d] = %lu\n", j, bitmap_sizes[j]);
			}

			char *bitmap_data = malloc(bitmap_sizes[format & GLYPH_PAD_MASK]);
			fread(bitmap_data, bitmap_sizes[format & GLYPH_PAD_MASK], 1, fp);

			printf("format: %02x\n", format);
			printf("bitmap size: %lu\n", bitmap_sizes[format & GLYPH_PAD_MASK]);

			int reverse_bits = !!(format & PCF_BIT_MASK);
			int reverse_bytes = !!(format & PCF_BYTE_MASK);
			int bytes_per_row = 1 << (format & PCF_GLYPH_PAD_MASK);

			//
			// Now actually write the font_t data.
			//

			font->bitmap_size = bitmap_sizes[bitmap_sizes[1]];
			font->bitmap_data = malloc(font->bitmap_size);
			memset(font->offsets, 0xff, sizeof(font->offsets));

			for(j = 0; j < glyph_count; j++) {
				printf("glyph: %d offs %ld\n", j, (int32_t)offsets[j]);
				
				int k;
				for(k = offsets[j]; k < offsets[j+1]; k+=bytes_per_row) {

					int byte;
					int byte_start, byte_end, byte_offs;
					if(reverse_bytes) {
						byte_start = bytes_per_row - 1;
						byte_offs = -1;
						byte_end = -1;
					}
					else {
						byte_start = 0;
						byte_offs = 1;
						byte_end = bytes_per_row;
					}

					for(byte = byte_start; byte != byte_end; byte += byte_offs) {
						int l;
						if(reverse_bits) {
							for(l = 7; l >= 0; l--) {
								putchar((bitmap_data[k+byte] >> l) & 0x01 ? '#' : ' ');
							}
						}
						else {
							for(l = 0; l < 8; l++) {
								putchar((bitmap_data[k+byte] >> l) & 0x01 ? '#' : ' ');
							}
						}
					}
					printf("\n");
				} // for k
			} // for j

		} // if PCF_BITMAPS
	}


	fclose(fp);
}

void main(int argc, char **argv) {
	printf("reading %s\n", argv[1]);
	pcf_read(argv[1]);
}

	// TODO: open file and stuff

//char header[4];			[> always "\1fcp" <]
//lsbint32 table_count;
//struct toc_entry {
	//uint32_t type;		[> See below, indicates which table <]
	//uint32_t format;		[> See below, indicates how the data are formatted in the table <]
	//uint32_t size;		[> In bytes <]
	//uint32_t offset;		[> from start of file <]
//} tables[table_count];
