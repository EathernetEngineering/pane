#include "ppu.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace pane {
PPU::PPU() {
	m_pPixels = reinterpret_cast<uint8_t*>(std::calloc(PANE_NES_VISIBLE_IMAGE_WIDTH * PANE_NES_VISIBLE_IMAGE_HEIGHT * 4, sizeof(uint8_t)));
	if (m_pPixels == nullptr) {
		throw std::runtime_error("Failed to allocate pixel buffer for PPU.");
	}
}

PPU::~PPU() {
	if (m_pPixels) {
		std::free(m_pPixels);
	}
}

void PPU::Execute() {
}
}

