
#include <iostream>
#include "pcf_font.h"
#include "canvas/canvas.h"
#include "canvas/buffer.h"


int main(int argc, char** argv) {
	using namespace pinball;

	if(argc < 2) {
		std::cerr << "syntax: " << argv[0] << " [pcf font path]" << std::endl;
		return 1;
	}

	PcfFont font { argv[1] };

	const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(){}[]/?=+-_\\|'\",<.>`~;:";

	auto size = font.get_char('a').size();

	std::cout
		<< "#include <map>\n"
		<< "#include <array>\n"
		<< "\n"
		;

	std::cout << "Coordinate<> size { " << size.row() << ", " << size.column() << " };\n";

	std::cout << "std::map<char, std::array<unsigned char, " << size.area() << ">> font = { \n";
	for(const char *c = chars; *c != 0; ++c) {
		std::cout << "{ " << (int)*c << ", {  // [" << *c << "]\n  ";
		const auto& canvas = font.get_char(*c);
		for(int row = 0; row < canvas.size().row(); ++row) {
			for(int column = 0; column < canvas.size().column(); ++column) {
				std::cout << std::setw(3) << (int)canvas.get_pixel({row, column}) << ",  ";
			}
			std::cout << "\n  ";
		}
		std::cout << "} },\n";
	}
	std::cout << "};\n";
}

