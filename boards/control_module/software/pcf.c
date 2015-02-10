
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

// TODO:
// Ziel: PCF file in font struct laden, dann
// funktionen haben wie
// display_render_text(font*, char*, int x, int y);
/*
struct font {
	uint16_t offsets[256];

	// kein glyph ist bei uns breiter als 16 bit
	uint16_t *bitmap_data;
}
*/


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

void pcf_read_toc_entry(FILE *fp, struct toc_entry *r) {
	r->type = pcf_read_lsbint32(fp);
	r->format = pcf_read_lsbint32(fp);
	r->size = pcf_read_lsbint32(fp);
	r->offset = pcf_read_lsbint32(fp);
}

void pcf_read(char *filename) {
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

			char *bitmap_data = malloc(bitmap_sizes[format & 3]);
			fread(bitmap_data, bitmap_sizes[format & 3], 1, fp);

			printf("format: %02x\n", format);
			printf("bitmap size: %lu\n", bitmap_sizes[format & 3]);

			int reverse_bits = !!(format & PCF_BIT_MASK);
			int reverse_bytes = !!(format & PCF_BYTE_MASK);
			int bytes_per_row = 1 << (format & PCF_GLYPH_PAD_MASK);

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
