
#ifndef PCF_FONT_H
#define PCF_FONT_H

#include <string>
#include "canvas.h"

class PcfFont {

	public:
		PcfFont(const std::string&);

		void render(const std::string&, Canvas& canvas);

		Canvas::Coordinate get_size(const std::string& text);


	private:
		const std::string& file_name;
};


#endif // PCF_FONT_H


