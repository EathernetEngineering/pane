#ifndef CEE_PANE_PPU_H_
#define CEE_PANE_PPU_H_

#include <cstdint>

#define PANE_NES_VISIBLE_IMAGE_WIDTH    256
#define PANE_NES_VISIBLE_IMAGE_HEIGHT   240
#define PANE_NES_SCANLINES_PER_FRAME    261
#define PANE_NES_DOTS_PER_SCANLINE      341

namespace pane {
class PPU {
public:
	PPU();
	~PPU();

	void Execute();

	const uint8_t* GetPixels() const { return m_pPixels; }

private:
	uint8_t* m_pPixels = nullptr;
};
}

#endif

