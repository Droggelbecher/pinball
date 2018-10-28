
#ifndef PCF_FONT_H
#define PCF_FONT_H

#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "coordinate.h"
#include "canvas/buffer.h"

namespace pinball {

template<const Coordinate<>& SIZE>
class Font {

	private:

		using CharData = std::array<uint8_t, SIZE.area()>;
		using FontData = std::map<char, CharData>;

	public:
		class CharacterCanvas {
			public:
				static const canvas::DataOrder data_order = canvas::COLUMN_FIRST;
				CharacterCanvas(const CharData& character)
					: character(character) {
				}

				int buffer_length() const { return SIZE.area(); }
				const uint8_t* buffer() const { return character.data(); }
				Coordinate<> size() const { return SIZE; }

			private:
				const CharData& character;

		}; // class CharacterCanvas

		Font(const FontData& font_data) : font_data(font_data) {}
		Coordinate<> size(const std::string& text);

		template<typename Canvas>
		void paint_char(Canvas& canvas, char ch, Coordinate<> c, uint8_t color) {

			/*
			CharacterCanvas cc(font_data.at(ch));
			for(int j=0; j<11; j++) {
				for(int i=0; i <5; i++) {
					std::cout << (int)cc.buffer()[i + j * 5];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			*/
			//assert(false);
			
			blit(
					CharacterCanvas(font_data.at(ch)),
					canvas,
					Coordinate<>(0, 0),
					SIZE, // - Coordinate<>(1, 1),
					c
			);
		}

		template<typename Canvas>
		void paint_string(Canvas& canvas, const char *s,  Coordinate<> start, uint8_t color) {
			//paint_char(canvas, 'a', start, 1);
			for( ; *s; ++s) {
				paint_char(canvas, *s, start, 1);
				start.column() += SIZE.column() + 1;
			}
		}

	private:
		const FontData& font_data;
};

} // namespace pinball

#endif // PCF_FONT_H


/* vim: set ts=2 sw=2 tw=78 noexpandtab :*/
