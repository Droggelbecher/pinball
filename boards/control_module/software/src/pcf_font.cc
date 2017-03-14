
#include "pcf_font.h"

#include <fstream>
#include <iostream>

#include <utils.h>

namespace {

	struct LSB {
		static int shift(int i, int sz) {
			return i * 8;
		}
	};

	struct MSB {
		static int shift(int i, int sz) {
			return (sz - i - 1) * 8;
		}
	};

	template<typename R, typename OrderTrait>
	R read(std::istream& is) {
		unsigned char buffer[sizeof(R)];
		is.read(reinterpret_cast<char*>(buffer), sizeof(buffer));

		R r = 0;
		for(int i = 0; i < sizeof(buffer); i++) {
			r |= buffer[i] << OrderTrait::shift(i, sizeof(buffer));
		}
		return r;
	}

	uint16_t read_lsb_u16(std::istream& is) { return read<uint16_t, LSB>(is); }
	uint16_t read_msb_u16(std::istream& is) { return read<uint16_t, MSB>(is); }
	uint32_t read_lsb_u32(std::istream& is) { return read<uint32_t, LSB>(is); }
	uint32_t read_msb_u32(std::istream& is) { return read<uint32_t, MSB>(is); }

} // namespace

uint32_t PcfFont::read_u32(std::istream& is, unsigned char format) {
	if(format & PcfFont::PCF_BYTE_MASK) {
		return read_msb_u32(is);
	}
	else {
		return read_lsb_u32(is);
	}
}

uint16_t PcfFont::read_u16(std::istream& is, unsigned char format) {
	if(format & PcfFont::PCF_BYTE_MASK) {
		return read_msb_u16(is);
	}
	else {
		return read_lsb_u16(is);
	}
}

PcfFont::PcfFont(const std::string& filename) : filename_(filename) {
	load_font(filename_.c_str());
}

void PcfFont::load_font(const char *filename) {
	std::ifstream pcf(filename);

	char header[4];

	pcf.read(header, sizeof(header));

	if(header[0] != '\1' || header[1] != 'f'
		 || header[2] != 'c' || header[3] != 'p') {

		// TODO: determine a consistent error handling strategy
		perror("not a PCF file.\n");
	}

	uint32_t table_count = read_lsb_u32(pcf);

	for(uint32_t i = 0; i < table_count; i++) {
		Entry e(pcf);

		std::ifstream::pos_type pos = pcf.tellg();
		pcf.seekg(e.offset);

		switch(e.type) {
			case PCF_BDF_ENCODINGS:
				encoding = Encoding(pcf);
				break;

			case PCF_BITMAPS:
				bitmaps = Bitmaps(pcf);
				break;

			default:
				break;
		} // sw

		pcf.seekg(pos);
	} // for i


	// Transform into internal format
	
	transformed_.bitmap_data = new uint8_t[bitmaps.bitmap_sizes[1]];

	int target_offset = 0;
	const int bytes_per_row = 1 << (bitmaps.format & PCF_GLYPH_PAD_MASK);
	const float size_factor = 2.0 / bytes_per_row;

	for(int glyph = 0; glyph < bitmaps.glyph_count - 1; glyph++) {
		int offset_start = bitmaps.offsets[glyph];
		int offset_end = bitmaps.offsets[glyph + 1];

		transform_bytes(bitmaps.bitmap_data + offset_start,
				bitmaps.bitmap_data + offset_end,
				transformed_.bitmap_data + target_offset,
				bitmaps.format);
		target_offset += (offset_end - offset_start) * size_factor;
	}

	for(int j = encoding.min_char_or_byte2; j <= encoding.max_char_or_byte2; j++) {
		uint16_t glyph_idx = encoding.glyphindeces[j - encoding.min_char_or_byte2];
		uint16_t offs = 0xffff;
		if(glyph_idx != 0xffff) {
			offs = bitmaps.offsets[glyph_idx];
		}
		if(offs != 0xffff) {
			offs *= size_factor;
		}
		transformed_.offsets[j] = offs;
	}

} // load_font()

void PcfFont::transform_bytes(uint8_t *source_start, uint8_t *source_end, uint8_t *target_start, uint32_t format) {
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

PcfFont::Entry::Entry(std::ifstream& pcf) {
	type = read_lsb_u32(pcf);
	format = read_lsb_u32(pcf);
	size = read_lsb_u32(pcf);
	offset = read_lsb_u32(pcf);
}

PcfFont::Encoding::Encoding()
		: glyphindeces(nullptr) {
}

PcfFont::Encoding::Encoding(std::ifstream& pcf)
		: glyphindeces(nullptr) {

	format = read_lsb_u32(pcf);
	min_char_or_byte2 = read_u16(pcf, format);
	max_char_or_byte2 = read_u16(pcf, format);
	min_byte1 = read_u16(pcf, format);
	max_byte1 = read_u16(pcf, format);
	default_char = read_u16(pcf, format);

	uint32_t sz = 2 * (max_char_or_byte2 - min_char_or_byte2 + 1) * (max_byte1 - min_byte1 + 1);
	glyphindeces = new uint16_t[sz];

	pcf.read(reinterpret_cast<char*>(glyphindeces), sz);
	if(!!(format & PCF_BYTE_MASK) != is_big_endian()) {
		swap_bytes(reinterpret_cast<uint8_t*>(glyphindeces), sz);
	}
}

PcfFont::Encoding::~Encoding() {
	delete glyphindeces;
}

PcfFont::Encoding& PcfFont::Encoding::operator=(Encoding&& other) {
	format = other.format;
	min_char_or_byte2 = other.min_char_or_byte2;
	max_char_or_byte2 = other.max_char_or_byte2;
	min_byte1 = other.min_byte1;
	max_byte1 = other.max_byte1;
	default_char = other.default_char;

	delete glyphindeces;
	glyphindeces = other.glyphindeces;
	other.glyphindeces = nullptr;
	return *this;
}

PcfFont::Bitmaps::Bitmaps()
	: offsets(nullptr), bitmap_data(nullptr) {
}

PcfFont::Bitmaps::Bitmaps(std::ifstream& pcf)
		: offsets(nullptr), bitmap_data(nullptr) {
	format = read_lsb_u32(pcf);
	glyph_count = read_u32(pcf, format);
	offsets = new uint32_t[glyph_count];
	for(int i = 0; i < glyph_count; i++) {
		offsets[i] = read_u32(pcf, format);
	}
	for(int i = 0; i < 4; i++) {
		bitmap_sizes[i] = read_u32(pcf, format);
	}
	uint32_t sz = bitmap_sizes[format & PCF_GLYPH_PAD_MASK];
	bitmap_data = new uint8_t[sz];
	// TODO This is dangerous and wrong if sizeof(char) != 1
	pcf.read(reinterpret_cast<char*>(bitmap_data), sz);
}

PcfFont::Bitmaps::~Bitmaps() {
	delete offsets;
	delete bitmap_data;
}


PcfFont::Bitmaps::Bitmaps(Bitmaps&& other) {
	*this = std::move(other);
}

PcfFont::Bitmaps& PcfFont::Bitmaps::operator=(Bitmaps&& other) {
	format = other.format;
	glyph_count = other.glyph_count;

	delete offsets;
	offsets = other.offsets;
	other.offsets = nullptr;

	std::copy(other.bitmap_sizes, other.bitmap_sizes + 4, bitmap_sizes);

	delete bitmap_data;
	bitmap_data = other.bitmap_data;
	other.bitmap_data = nullptr;

	return *this;
}

Coordinate<> PcfFont::size(const std::string& text) {
	// TODO
	return Coordinate<>();
}

bool PcfFont::paint_char(Canvas& canvas, char ch, Coordinate<> c, uint8_t color) {
	uint16_t offs = transformed_.offsets[(uint8_t)ch];
	bool r = false;
	if(offs == 0xffff) { return r; }

	for(uint16_t row = c.row(); offs < transformed_.offsets[(uint8_t)ch + 1]; offs += 2, row++) {
		int bit;
		for(bit = 0; bit < 8; bit++) {

			Coordinate<> cb(row, c.column() + bit);
			if((transformed_.bitmap_data[offs] & (1 << bit)) && canvas.canvas_size().contains(cb)) {
				canvas.set_pixel(cb, color);
				r = true;
			}

			cb = Coordinate<>(row + 1, c.column() + bit);
			if((transformed_.bitmap_data[offs + 1] & (1 << bit)) && canvas.canvas_size().contains(cb)) {
				canvas.set_pixel(cb, color);
				r = true;
			}
		}
	}
	return r;
}

bool PcfFont::paint_string(Canvas& canvas, const char *s,  Coordinate<> start, uint8_t color) {

	uint16_t width = 8;
	uint8_t r;

	for( ; *s != '\0'; s++) {
		r = paint_char(canvas, *s, start, color) || r;
		start += Coordinate<>(0, width);
	}
	return r;
}

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
