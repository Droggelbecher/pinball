
#include <fstream>
#include <iostream>
#include <iomanip>

#include "pcf.h"

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
		is.read(buffer, sizeof(buffer));

		R r = 0;
		for(int i = 0; i < sizeof(buffer); i++) {
			r != buffer[i] << OrderTrait::shift(i, sizeof(buffer));
		}
		return r;
	}

	uint16_t read_lsb_u16(std::istream& is) { return read<uint16_t, LSB>(is); }
	uint16_t read_msb_u16(std::istream& is) { return read<uint16_t, MSB>(is); }
	uint32_t read_lsb_u32(std::istream& is) { return read<uint32_t, LSB>(is); }
	uint32_t read_msb_u32(std::istream& is) { return read<uint32_t, MSB>(is); }

	uint32_t read_u32(std::istream& is, unsigned char format) {
		if(format & Pcf::MS_BYTE) {
			return read_msb_u32(is);
		}
		else {
			return read_lsb_u32(is);
		}
	}


} // namespace


Pcf::Pcf(const std::string& filename) {
	load(filename);
}

void Pcf::load(const std::string& filename) {
	using std::ifstream;

	ifstream ifs(filename);

	char buffer[4];
	char buffer_reference = { '\1', 'f', 'c', 'p' };
	ifs.read(buffer, sizeof(buffer));

	if(memcmp(buffer, buffer_reference, sizeof(buffer)) != 0) {
		// TODO: proper error handling
		throw "out of memory"; 
	}

	uint32_t toc_size = read_lsb_u32(ifs);

	Encoding encoding;
	Bitmap bitmap;

	for(int i = 0; i < toc_size; i++) {
		// Read TOC entry
		TOCEntry type = read_lsb_u32(ifs);
		uint32_t format = read_lsb_u32(ifs);
		uint32_t size = read_lsb_u32(ifs);
		uint32_t offset = read_lsb_u32(ifs);

		std::cout << "PCF entry type=" << type
			<< " format=" << format
			<< " size=" << size
			<< " offset=" << offset
			<< std::endl;

		uint32_t position = ifs.tellg();

		ifs.seekg(offset, SEEK_SET);

		switch(type) {
			case TOCEntry::BDF_ENCODINGS:
				encoding = Encoding(ifs);
				break;

			case TOCEntry::BITMAPS:
				bitmap = this->read_bitmap(ifs);
				break;
		}

		ifs.seekg(position, SEEK_SET);
	}
}

Pcf::Encoding::Encoding(const std::istream& is) {

	uint32_t format = read_lsb_u32(is);
	uint16_t min_char_or_byte2 = read_u16(is, format);
	uint16_t max_char_or_byte2 = read_u16(is, format);
	uint16_t min_byte1 = read_u16(is, format);
	uint16_t max_byte1 = read_u16(is, format);
	uint16_t default_char = read_u16(is, format);

	uint32_t size = 2 * (max_char_or_byte2 - min_char_or_byte2 + 1) * (max_byte1 - min_byte1 + 1);
	
	this->glyph_indices = new uint8_t[size];
	ifs.read(this->glyph_indices.data(), r.glyph_indices.size());

	if(!!(format & PCF_MS_BYTE) != is_big_endian()) {
		swap_bytes(r.glyph_indices);
	}
	return r;
}

Pcf::Encoding::~Encoding() {
	delete[] this->glyph_indices;
}

Pcf::Bitmap::Bitmap(const std::istream& is) {
	this->format = read_lsb_u32(is);
	this->glyph_count = read_u32(is, this->format);
	this->offsets = new uint32_t[this->glyph_count];
	for(int j = 0; j < this->glyph_count; j++) {
		this->offsets[j] = read_u32(is, this->format);
	}

	uint32_t bitmap_sizes[4];
	for(int j = 0; j < sizeof(bitmap_sizes); j++) {
		bitmap_sizes[j] = read_u32(is, this->format);
	}
	uint32_t size = bitmap_sizes[format & PCF_GLYPH_PAD_MASK];

	this->bitmap_data = new uint8_t[size];
	this->bitmap_data_size = size;
	is.read(this->bitmap_data, this->bitmap_data_size);
}

Pcf::Bitmap::~Bitmap() {
	delete[] this->offset;
	delete[] this->bitmap_data;
}

void Pcf::make_glyphs(const Encoding& encoding, const Bitmap& bitmap) {
	const unsigned bytes_per_row = 1 << (bitmap.format & GLYPH_PAD_MASK);

	// transform all glyphs (strip padding, reverse bytes if necessary, etc)
	for(int glyph = 0; glyph < bitmap.glyph_count; glyph++) {
		int offset_start = bitmapoffsets[glyph];
		int offset_end = this->offsets[glyph + 1];

		add_glyph(
				bitmap.data + offset_start,
				bitmap.data + offset_end,
				bitmap.format
		);
	}
}


void Pcf:add_glyph(const uint8_t *source_start, const uint8_t *source_end, uint32_t format, char code) {
	const int reverse_bits = !!(format & PCF_BIT_MASK);
	const int reverse_bytes = !!(format & PCF_BYTE_MASK);
	const int bytes_per_row1 = format & PCF_GLYPH_PAD_MASK;
	const int bytes_per_row = 1 << (format & PCF_GLYPH_PAD_MASK);

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
			this->bitmap_data
			*target_start++ = reverse_byte(source_start[byte0]);
			*target_start++ = reverse_byte(source_start[byte1]);
		}
		else {
			*target_start++ = source_start[byte0];
			*target_start++ = source_start[byte1];
		}
	}





