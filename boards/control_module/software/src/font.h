
#ifndef PCF_FONT_H
#define PCF_FONT_H

#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "coordinate.h"
#include "canvas/buffer.h"

namespace pinball {

enum Alignment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

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
		Coordinate<> size(const std::string& text) {
			return { SIZE.row(), static_cast<int>((SIZE.column() + 1) * text.size()) };
		}

		template<typename Canvas>
		void paint_char(Canvas& canvas, char ch, Coordinate<> c, uint8_t color) {
			blit(
					CharacterCanvas(font_data.at(ch)),
					canvas,
					Coordinate<>(0, 0),
					SIZE, // - Coordinate<>(1, 1),
					c
			);
		}

		template<typename Canvas>
		void paint_string(Canvas& canvas, const char *s,  Coordinate<> start, uint8_t color, Alignment align = ALIGN_LEFT) {
			if(align == ALIGN_RIGHT) {
				start = { start.row(), canvas.size().column() - this->size(s).column() };
			}
			else if(align == ALIGN_CENTER) {
				start = {
					start.row(),
					static_cast<int>((start.column() + canvas.size().column() - this->size(s).column()) / 2)
				};
			}

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
