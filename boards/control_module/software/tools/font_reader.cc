
#include <iostream>
#include <fstream>
#include <string>
#include "pcf_font.h"
#include "canvas/canvas.h"
#include "canvas/buffer.h"

using namespace pinball;

void write_header(PcfFont& font, std::ofstream& ofs) {
	auto size = font.get_char('a').size();

	ofs
		<< "#include <map>\n"
		<< "#include <array>\n"
		<< "#include \"coordinate.h\"\n"
		<< "\n"
		<< "namespace pinball {\n"
		<< "  constexpr Coordinate<> g_font_size { "
			<< size.row() << ", " << size.column() << " };\n"
		<< "  extern const std::map<char, std::array<uint8_t, " << size.area() << ">> g_font_data;\n"
		<< "}\n"
		<< "\n\n"
		;
}

void write_source(PcfFont& font, std::string h_filename, std::ofstream& ofs) {
	auto size = font.get_char('a').size();

	const char *chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(){}[]/?=+-_\\|'\",<.>`~;:";

	ofs << "#include \"" << h_filename << "\"\n"
		<< "\n"
		<< "namespace pinball {\n"
		<< "\n";

	ofs << "const std::map<char, std::array<uint8_t, " << size.area() << ">> g_font_data = { \n";
	for(const char *c = chars; *c != 0; ++c) {
		ofs << "{ " << (int)*c << ", {  // [" << *c << "]\n  ";
		const auto& canvas = font.get_char(*c);
		for(int row = 0; row < canvas.size().row(); ++row) {
			for(int column = 0; column < canvas.size().column(); ++column) {
				ofs << std::setw(3) << (int)canvas.get_pixel({row, column}) << ",  ";
			}
			ofs << "\n  ";
		}
		ofs << "} },\n";
	}
	ofs << "};\n";

	ofs << "} // namespace pinball\n\n";
}

int main(int argc, char** argv) {

	if(argc < 4) {
		std::cerr << "syntax: " << argv[0] << " [pcf font path] [outfile.cc] [outfile.h]" << std::endl;
		return 1;
	}

	PcfFont font { argv[1] };

	std::ofstream cc(argv[2]);
	write_source(font, argv[3], cc);
	cc.close();

	std::ofstream h(argv[3]);
	write_header(font, h);
	h.close();
}

