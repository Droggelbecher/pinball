
#include "pcf_font.h"

#include <fstream>

PcfFont::PcfFont(const std::string& filename) : filename_(filename) {
}

uint16_t PcfFont::read_lsbint16(std::ifstream& f) {
	uint16_t r = 0;
	char c = '\0';;
	f.get(c); r = static_cast<uint16_t>(c);
	f.get(c); r |= static_cast<uint16_t>(c) << 8;
	return r;
}

uint16_t PcfFont::read_msbint16(std::ifstream& f) {
	uint16_t r = 0;
	char c = '\0';;
	f.get(c); r = static_cast<uint16_t>(c) << 8;
	f.get(c); r |= static_cast<uint16_t>(c);
	return r;
}

uint32_t PcfFont::read_lsbint32(std::ifstream& f) {
	uint32_t r = 0;
	char c = '\0';;
	f.get(c); r = static_cast<uint32_t>(c);
	f.get(c); r |= static_cast<uint32_t>(c) << 8;
	f.get(c); r |= static_cast<uint32_t>(c) << 16;
	f.get(c); r |= static_cast<uint32_t>(c) << 24;
	return r;
}

uint32_t PcfFont::read_msbint32(std::ifstream& f) {
	uint32_t r = 0;
	char c = '\0';;
	f.get(c); r = static_cast<uint32_t>(c) << 24;
	f.get(c); r |= static_cast<uint32_t>(c) << 16;
	f.get(c); r |= static_cast<uint32_t>(c) << 8;
	f.get(c); r |= static_cast<uint32_t>(c);
	return r;
}

uint32_t PcfFont::read_int32(std::ifstream& f, unsigned char format) {
	if(format & PCF_BYTE_MASK) {
		return PcfFont::read_msbint32(f);
	}
	else {
		return PcfFont::read_lsbint32(f);
	}
}

uint16_t PcfFont::read_int16(std::ifstream& f, unsigned char format) {
	if(format & PCF_BYTE_MASK) {
		return PcfFont::read_msbint16(f);
	}
	else {
		return PcfFont::read_lsbint16(f);
	}
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

	uint32_t table_count = read_lsbint32(pcf);

	Encoding encoding;
	Bitmaps bitmaps;

	for(uint32_t i = 0; i < table_count; i++) {
		Entry e = read_toc_entry(pcf);

		std::ifstream::pos_type pos = pcf.tellg();
		pcf.seekg(e.offset);

		switch(entry.type) {
			case PCF_BDF_ENCODING:
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
} // load_font()

PcfFont::Encoding::Encoding(std::ifstream& pcf)
		: glyphindeces(nullptr) {

	format = read_lsbint32(pcf);
	min_char_or_byte2 = read_int16(pcf, format);
	max_char_or_byte2 = read_int16(pcf, format);
	min_byte1 = read_int16(pcf, format);
	max_byte1 = read_int16(pcf, format);
	default_char = read_int16(pcf, format);

	uint32_t sz = 2 * (max_char_or_byte2 - min_char_or_byte2 + 1) * (max_byte1 - min_byte1 + 1);
	glyphindeces = new uint16_t[sz];

	pcf.read(glyphindeces, sz);
	if(!!(format & PCF_BYTE_MASK) != is_big_endian()) {
		swap_bytes((uint8_t*)glyphindeces, sz);
	}
}

PcfFont::Encoding::~Encoding() {
	delete glyphindeces;
}

PcfFont::Encoding::operator=(Encoding&& other) {
	format = other.format;
	min_char_or_byte2 = other.min_char_or_byte2;
	max_char_or_byte2 = other.max_char_or_byte2;
	min_byte1 = other.min_byte1;
	max_byte1 = other.max_byte1;
	default_char = other.default_char;

	delete glyphindeces;
	glyphindeces = other.glyphindeces;
	other.glyphindeces = nullptr;
}

PcfFont::Bitmaps::Bitmaps(std::ifstream& pcf)
		: offsets(nullptr), bitmap_data(nullptr) {
	format = read_lsbint32(pcf);
	glyph_count = read_int32(pcf, format);
	offsets = new uint32_t[glyph_count];
	for(int i = 0; i < glyph_count; i++) {
		offsets[i] = read_int32(pcf, format);
	}
	for(int i = 0; i < 4; i++) {
		bitmap_sizes[i] = read_int32(pcf, format);
	}
	uint32_t sz = bitmap_sizes[format & PCF_GLYPH_PAD_MASK];
	bitmap_data = new uint8_t[sz];
	pcf.read(bitmap_data, sz);
}

PcfFont::Bitmaps::~Bitmaps() {
	delete offsets;
	delete bitmap_data;
}

Bitmaps& PcfFont::Bitmaps::Bitmaps(Bitmaps&& other) {
	format = other.format;
	glyph_count = other.glyph_count;

	delete offsets;
	offsets = other.offsets;
	other.offsets = nullptr;

	delete bitmap_sizes;
	bitmap_sizes = other.bitmap_sizes;
	bitmap_sizes = nullptr;
}

Coordinate<> PcfFont::size(const std::string& text) {
	// TODO
	return Coordinate<>();
}

Coordinate<> PcfFont::render(const std::string& text, Canvas& canvas) {
	// TODO
	return Coordinate<>();
}

/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
