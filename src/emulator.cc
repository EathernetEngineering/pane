#include "emulator.h"
#include "event.h"

#include <chrono>
#include <vector>
#include <random>

namespace pane {
Emulator::Emulator() {

}

Emulator::~Emulator() {

}

void Emulator::Init() {
	m_pMMU = std::make_shared<MMU>();
	m_pCPU = std::make_shared<CPU>();
	m_pPPU = std::make_shared<PPU>();

	m_pMMU->Init();

	m_pCPU->SetMMU(m_pMMU);
	m_pPPU->SetMMU(m_pMMU);
	m_pPPU->SetCPU(m_pCPU);

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

	m_pMMU->Shutdown();

	m_pMMU.reset();
}
	
void Emulator::Run() {
	m_pCPU->Start();
	std::shared_ptr<Event> e;
	while (!m_pWindow->ShouldClose()) {
		while (!m_pPPU->ShouldRender()) {
			m_pPPU->Execute();
			m_pPPU->Execute();
			m_pPPU->Execute();
			m_pCPU->Execute();
		}

		m_pRenderer->UpdateImage(m_pPPU->GetPixels());
		m_pRenderer->RenderFrame();
		m_pWindow->SwapBuffers();
		m_pPPU->Rendered();

		while ((e = m_pWindow->PollEvents()).get() != nullptr) {
			// Handle event
		}
	}
}
}

