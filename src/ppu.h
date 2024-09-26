#ifndef CEE_PANE_PPU_H_
#define CEE_PANE_PPU_H_

#include <memory>

#include <cstdint>

#include "mmu.h"
#include "cpu.h"

#define PANE_NES_VISIBLE_IMAGE_WIDTH    256
#define PANE_NES_VISIBLE_IMAGE_HEIGHT   240
#define PANE_NES_SCANLINES_PER_FRAME    261
#define PANE_NES_DOTS_PER_SCANLINE      341

namespace pane {
class PPU {
public:
	PPU();
	~PPU();

	void SetMMU(std::shared_ptr<MMU> pMMU);
	void SetCPU(std::shared_ptr<CPU> pCPU);

	void Execute();
	bool ShouldRender() const { return m_bRender; }
	void Rendered() { m_bRender = false; }

	const uint8_t* GetPixels() const { return m_pPixels; }

private:
	std::shared_ptr<MMU> m_pMMU;
	std::shared_ptr<CPU> m_pCPU;
	uint8_t* m_pPixels = nullptr;

	bool m_bRender = 0;
};
}

#endif

