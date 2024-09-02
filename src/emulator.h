#ifndef CEE_PANE_EMULATOR_H_
#define CEE_PANE_EMULATOR_H_

#include <memory>

#include "ram.h"
#include "cpu.h"
#include "ppu.h"
#include "window.h"
#include "renderer.h"

namespace pane {
class Emulator {
public:
	Emulator();
	~Emulator();

	void Init();
	void Shutdown();
	
	void Run();

private:
	std::shared_ptr<RAM> m_pRAM;
	std::shared_ptr<CPU> m_pCPU;
	std::shared_ptr<PPU> m_pPPU;

	std::shared_ptr<Window> m_pWindow;
	std::unique_ptr<Renderer> m_pRenderer;
};
}

#endif

