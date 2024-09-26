#include "ppu.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>

int g_nCounter;

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
		m_pPixels = nullptr;
	}
}

void PPU::SetMMU(std::shared_ptr<MMU> pMMU) {
	m_pMMU = pMMU;
}

void PPU::SetCPU(std::shared_ptr<CPU> pCPU) {
	m_pCPU = pCPU;
}

void PPU::Execute() {
	if (m_bRender) {
		g_nCounter = 0;
	}
	g_nCounter++;
	if (g_nCounter > 29780) {
		m_bRender = true;
	}
}
}

