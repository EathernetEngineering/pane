#include "emulator.h"
#include <memory>

namespace pane {

Emulator::Emulator() {

}

Emulator::~Emulator() {

}

void Emulator::Init() {
	m_pRAM = std::make_shared<RAM>();
	m_pCPU = std::make_shared<CPU>();
	m_pPPU = std::make_shared<PPU>();

	m_pCPU->SetRAM(m_pRAM);

	m_pWindow = std::make_shared<Window>();
	m_pWindow->Init(1280, 720, "pane");

	m_pRenderer = std::make_unique<Renderer>();
	m_pRenderer->Init();
}

void Emulator::Shutdown() {
	m_pRenderer->Shutdown();
	m_pRenderer.reset();
	m_pWindow->Shutdown();
	m_pWindow.reset();

	m_pPPU.reset();
	m_pCPU->Reset();
	m_pCPU.reset();
	m_pRAM.reset();
}
	
void Emulator::Run() {
	while (!m_pWindow->ShouldClose()) {
		m_pCPU->Execute();

		m_pRenderer->RenderFrame();
		m_pWindow->SwapBuffers();
		m_pWindow->PollEvents();
	}
}
}

