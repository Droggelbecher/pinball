
#include <iostream>
#include "pcf_font.h"
#include "canvas/canvas.h"
#include "canvas/buffer.h"


int main(int argc, char** argv) {
	using namespace pinball;

	PcfFont font { "resources/gohufont-11.pcf" };

	const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789§±!@#$%^&*(){}[]/?=+-_\\|'\",<.>`~;:";

	std::cout << "std::map<char, uint8_t[]> = { \n";
	for(const char *c = chars; *c != 0; ++c) {
		std::cout << "{ '" << *c << "',\n  ";
		const auto& canvas = font.get_char(*c);
		for(int row = 0; row < canvas.size().row(); ++row) {
			for(int column = 0; column < canvas.size().column(); ++column) {
				std::cout << std::setw(3) << canvas.get_pixel({row, column}) << ",  ";
			}
			std::cout << "\n  ";
		}
		std::cout << "} },\n";
	}
	std::cout << "};\n";
}

