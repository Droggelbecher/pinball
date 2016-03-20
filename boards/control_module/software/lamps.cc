
#include "lamps.h"

#include "config.h"

#include <checksum.h>
#include <cstring> // memset

void Lamps::write() {
  state_[8] = checksum(state_, 8);
  spi_.enable_slave(SPI_SS_PIN_LAMPS);
  spi_.readwrite(9, state_, 0);
}

void Lamps::set(Index index, bool v) {
  unsigned idx = static_cast<unsigned>(index);
  if(v) {
    state_[idx / 8] |= 1 << (idx % 8);
  }
  else {
    state_[idx / 8] &= ~(1 << (idx % 8));
  }
}

void Lamps::clear() {
  memset(state_, 0x00, sizeof(state_));
}

