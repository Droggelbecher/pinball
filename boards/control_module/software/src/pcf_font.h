
#ifndef PCF_FONT_H
#define PCF_FONT_H

#include <string>
#include "canvas/canvas.h"

class PcfFont {

		enum {
			PCF_PROPERTIES         =  (1<<0),
			PCF_ACCELERATORS       =  (1<<1),
			PCF_METRICS            =  (1<<2),
			PCF_BITMAPS            =  (1<<3),
			PCF_INK_METRICS        =  (1<<4),
			PCF_BDF_ENCODINGS      =  (1<<5),
			PCF_SWIDTHS            =  (1<<6),
			PCF_GLYPH_NAMES        =  (1<<7),
			PCF_BDF_ACCELERATORS   =  (1<<8),

			PCF_DEFAULT_FORMAT     =  0x00000000,
			PCF_INKBOUNDS          =  0x00000200,
			PCF_ACCEL_W_INKBOUNDS  =  0x00000100,
			PCF_COMPRESSED_METRICS =  0x00000100,

			PCF_GLYPH_PAD_MASK     =  (3<<0),       /* See the bitmap table for explanation */
			PCF_BYTE_MASK          =  (1<<2),       /* If set then Most Sig Byte First */
			PCF_BIT_MASK           =  (1<<3),       /* If set then Most Sig Bit First */
			PCF_SCAN_UNIT_MASK     =  (3<<4),       /* See the bitmap table for explanation */

			PCF_NOGLYPH            =  0xffff
		};

		struct Entry {
			public:
				Entry(std::ifstream& f);

				uint32_t type;
				uint32_t format;
				uint32_t size;
				uint32_t offset;
		};

		class Encoding {
			public:
				Encoding() { }
				Encoding(std::ifstream& f);
				~Encoding();
				Encoding& operator=(Encoding&& other);

				uint32_t format;
				uint16_t min_char_or_byte2;
				uint16_t max_char_or_byte2;
				uint16_t min_byte1;
				uint16_t max_byte1;
				uint16_t default_char;
				uint16_t *glyphindeces;
		};

		class Bitmaps {
			public:
				Bitmaps() { }
				Bitmaps(std::ifstream& f);
				~Bitmaps();
				Bitmaps(Bitmaps&&);
				Bitmaps& operator=(Bitmaps&& other);

				uint32_t format;
				uint32_t glyph_count;
				uint32_t *offsets;
				uint32_t bitmap_sizes[4];
				uint8_t *bitmap_data;
		};

	public:
		PcfFont(const std::string& filename);

		Coordinate<> render(const std::string&, Canvas& canvas);
		Coordinate<> size(const std::string& text);


	private:

		void load_font(const char *filename);

		static uint32_t read_u32(std::istream& is, unsigned char format);
		static uint16_t read_u16(std::istream& is, unsigned char format);
		//static uint16_t read_lsbint16(std::ifstream& f);
		//static uint16_t read_msbint16(std::ifstream& f);
		//static uint16_t read_int16(std::ifstream& f, unsigned char format);
		//static uint32_t read_lsbint32(std::ifstream& f);
		//static uint32_t read_msbint32(std::ifstream& f);
		//static uint32_t read_int32(std::ifstream& f, unsigned char format);

		// TODO: other read_... methods

		const std::string& filename_;
};


#endif // PCF_FONT_H


/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
