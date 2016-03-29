
#include "spi_display.h"


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

void SpiDisplay::next_frame() {
	spi_.enable_slave(Spi::DISPLAY);

	int i;
	for(i = 0; i < DISPLAY_MODULE_COUNT; i++) {
		spi_.transfer(modules_ * module_size_.area(), display_screen_ + i * module_size_.area(), nullptr);

		// Give display module a little time to enable next module
		usleep(10);
	}
	
	spi_.disable_slaves();
}

void SpiDisplay::set(Coordinate<> c, uint8_t color) {
	const unsigned char module = column / module_size_.column();
	const unsigned char col = column % module_size_.column();

	// screen[module][colors][rows][columns]
	
	assert(0 <= row && row < module_size_.row());
	assert(0 <= col && col < module_size_.column());
	assert(0 <= module && module < modules_);

	unsigned char *r = display_screen_ +
		(modules_ - module - 1) * (module_size_.column() * module_size_.row()) +
		row * (module_size_.column()) +
		(module_size_.column() - col - 1);

	assert(r >= display_screen_ && r < (display_screen_ + sizeof(display_screen_)));

	return r;
}

void SpiDisplay::get(Coordinate<> c) const {
	throw std::exception("Not implemented");
}

int SpiDisplay::buffer_length() const {
	return modules_ * module_size_.area();
}

uint8_t* SpiDisplay::buffer() {
	return display_screen_;
}

