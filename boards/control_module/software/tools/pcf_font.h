
#ifndef PCF_FONT_H
#define PCF_FONT_H

#include <string>
#include "coordinate.h"
#include "canvas/buffer.h"
#include <map>

namespace pinball {

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
				Encoding();
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
				Bitmaps();
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
		Coordinate<> size(const std::string& text);

		const canvas::Buffer& get_char(int c) {
			return characters.at(c);
		}

		template<typename Canvas>
		bool paint_char(Canvas& canvas, char ch, Coordinate<> c, uint8_t color) {
			// TODO
		}

		template<typename Canvas>
		bool paint_string(Canvas& canvas, const char *s,  Coordinate<> start, uint8_t color) {
			// TODO

		}

	private:

		void load_font(const char *filename);

		static uint32_t read_u32(std::istream& is, unsigned char format);
		static uint16_t read_u16(std::istream& is, unsigned char format);

		static canvas::Buffer render_char(uint8_t *source_begin, uint8_t *source_end, uint32_t format);

		static void transform_bytes(uint8_t *source_start, uint8_t *source_end, uint8_t *target_start, uint32_t format);

		const std::string& filename_;
		Bitmaps bitmaps;
		Encoding encoding;

		std::map<int, canvas::Buffer> characters;

		struct Transformed {
			uint8_t *bitmap_data;
			uint32_t offsets[256];
		};

		Transformed transformed_;

};

} // namespace pinball

#endif // PCF_FONT_H


/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
