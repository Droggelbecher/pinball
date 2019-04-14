

#include <cassert>
#include <string>
#include <cstring> // memset

#include "spi_display.h"

namespace pinball {

SpiDisplay::SpiDisplay(Spi& spi, uint8_t modules, Coordinate<> module_size)
	: spi_(spi), modules_(modules), module_size_(module_size) {

	display_screen_ = new uint8_t[modules_ * module_size_.area()];
}

SpiDisplay::~SpiDisplay() {
	delete[] display_screen_;
}

Coordinate<> SpiDisplay::size() const {
	return module_size_ * Coordinate<>(1, modules_);
}

void SpiDisplay::next_frame(double dt) {
	spi_.enable_only(Spi::DISPLAY);

	int i;
	for(i = 0; i < modules_; i++) {
		spi_.transfer(modules_ * module_size_.area(), display_screen_ + i * module_size_.area(), nullptr);

		// Give display module a little time to enable next module
		usleep(10);
	}
	
	spi_.disable_all();
}

void SpiDisplay::clear() {
	static const unsigned char size = modules_ * module_size_.area();
	memset(display_screen_, 0x00, size);
}


void SpiDisplay::set_pixel(Coordinate<> c, uint8_t color) {
	static const unsigned char module_area = module_size_.area();
	static const unsigned char module_cols = module_size_.column();
	static const unsigned char module_rows = module_size_.row();

	const unsigned char module = c.column() / module_cols;
	static const unsigned char col = c.column() % module_cols;

	// screen[module][colors][rows][columns]
	
	assert(0 <= c.row() && c.row() < module_size_.row());
	assert(0 <= col && col < module_size_.column());
	assert(0 <= module && module < modules_);

	uint8_t *r = display_screen_ +
		(modules_ - module - 1) * module_area +
		c.row() * module_cols +
		(module_cols - col - 1);

	assert(r >= display_screen_);
	assert(r < (display_screen_ + modules_ * module_size_.area()));

	*r = color;
}

uint8_t SpiDisplay::get_pixel(Coordinate<> c) const {
	throw std::exception();
}

int SpiDisplay::buffer_length() const {
	return modules_ * module_size_.area();
}

uint8_t* SpiDisplay::buffer() {
	return display_screen_;
}

} // ns pinball

