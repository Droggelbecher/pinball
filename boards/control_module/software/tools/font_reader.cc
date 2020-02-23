
#include <iostream>
#include <fstream>
#include <string>
#include "pcf_font.h"
#include "canvas/canvas.h"
#include "canvas/buffer.h"

using namespace pinball;

void write_header(PcfFont& font, std::ofstream& ofs, std::string name) {
	auto size = font.get_char('a').size();

	ofs
		<< "#include <map>\n"
		<< "#include <array>\n"
		<< "#include \"coordinate.h\"\n"
		<< "\n"
		<< "namespace pinball {\n"
		<< "  constexpr Coordinate<> font_" << name << "_size { "
			<< size.row() << ", " << size.column() << " };\n"
		<< "  extern const std::map<char, std::array<uint8_t, " << size.area() << ">> font_" << name << "_data;\n"
		<< "}\n"
		<< "\n\n"
		;
}

void write_source(PcfFont& font, std::ofstream& ofs, std::string name, std::string h_filename) {
	auto size = font.get_char('a').size();

	const char *chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(){}[]/?=+-_\\|'\",<.>`~;:";

	ofs << "#include \"" << h_filename << "\"\n"
		<< "\n"
		<< "namespace pinball {\n"
		<< "\n";

	ofs << "const std::map<char, std::array<uint8_t, " << size.area() << ">> font_" << name << "_data = { \n";
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

void write_d(PcfFont& font, std::ofstream& ofs, std::string name) {
	auto size = font.get_char('a').size();

	const char *chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(){}[]/?=+-_\\|'\",<.>`~;:";

	ofs << "import coordinate;\n\n";

	ofs << "enum font_" << name << "_size = Coordinate!()(" << size.row() << ", " << size.column() << ");\n\n";
	ofs << "ubyte[" << size.area() << "][char] font_" << name << "_data;\n";

	ofs << "static this() {\n";
	ofs << "font_" << name << "_data = [\n";
	for(const char *c = chars; *c != 0; ++c) {
		ofs << (int)*c << ": [  // [" << *c << "]\n  ";
		const auto& canvas = font.get_char(*c);
		for(int row = 0; row < canvas.size().row(); ++row) {
			for(int column = 0; column < canvas.size().column(); ++column) {
				ofs << std::setw(3) << (int)canvas.get_pixel({row, column}) << ",  ";
			}
			ofs << "\n  ";
		}
		ofs << "],\n";
	}
	ofs << "];\n";
	//ofs << "return data;\n";
	ofs << "}\n\n";

}


int main(int argc, char** argv) {

	if(argc < 4) {
		std::cerr << "syntax: " << argv[0] << " [pcf font path] [out path] [out name]" << std::endl;
		return 1;
	}

	PcfFont font { argv[1] };

	std::string cc_path = std::string(argv[2]) + "/" + argv[3] + ".cc";
	std::string h_path = std::string(argv[2]) + "/" + argv[3] + ".h";
	std::string d_path = std::string(argv[2]) + "/" + argv[3] + ".d";

	std::ofstream cc(cc_path);
	write_source(font, cc, argv[3], h_path);
	cc.close();

	std::ofstream h(h_path);
	write_header(font, h, argv[3]);
	h.close();

	std::ofstream d(d_path);
	write_d(font, d, argv[3]);
	d.close();
}

