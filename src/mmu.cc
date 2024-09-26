#include "mmu.h"

#include <cstring>
#include <cstdlib>
#include <stdexcept>

namespace pane {
MMU::MMU()
 : m_pRAM(nullptr), m_pCartridge(nullptr), m_pPPURegs(nullptr), m_pAPURegs(nullptr), m_pAPURegsUnused(nullptr), m_bInitialized(false)
{
}	

MMU::~MMU() {
	if (m_bInitialized) {
		this->Shutdown();
	}
}

void MMU::Init() {
	m_pRAM = reinterpret_cast<uint8_t*>(std::calloc(1, 0x0800));
	m_pCartridge = reinterpret_cast<uint8_t*>(std::calloc(1, 0xFFFF - 0x4020));
	m_pPPURegs = reinterpret_cast<uint8_t*>(std::calloc(1, 0x0008));
	m_pAPURegs = reinterpret_cast<uint8_t*>(std::calloc(1, 0x0018));
	m_pAPURegsUnused = reinterpret_cast<uint8_t*>(std::calloc(1, 0x0008));

	m_bInitialized = true;
}

void MMU::Shutdown() {
	if (m_pRAM) {
		std::free(m_pRAM);
		m_pRAM = nullptr;
	}
	if (m_pCartridge) {
		std::free(m_pCartridge);
		m_pCartridge = nullptr;
	}
	if (m_pPPURegs) {
		std::free(m_pPPURegs);
		m_pPPURegs = nullptr;
	}
	if (m_pAPURegs) {
		std::free(m_pAPURegs);
		m_pAPURegs = nullptr;
	}
	if (m_pAPURegsUnused) {
		std::free(m_pAPURegsUnused);
		m_pAPURegsUnused = nullptr;
	}
	m_bInitialized = false;
}

uint8_t MMU::Read(uint16_t pAddress) {
	if (!m_bInitialized) {
		return 0xFF;
	}

	if (pAddress < 0x2000) { // Internal RAM
		// 0x0800 - 0x0FFF Mirrors 0x0000 - 0x007FF
		// 0x1000 - 0x07FF Mirrors 0x0000 - 0x007FF
		// 0x1800 - 0x1FFF Mirrors 0x0000 - 0x007FF
		pAddress %= 0x0800;

		return *(m_pRAM + pAddress);
	} else if (pAddress < 0x4000) {
		pAddress -= 0x2000;
		pAddress %= 0x0008;
		return *(m_pPPURegs + pAddress);
	} else if (pAddress < 0x4018) {
		pAddress -= 0x4000;
		return *(m_pAPURegs + pAddress);
	} else if (pAddress < 0x4020) {
		pAddress -= 0x4018;
		return *(m_pAPURegsUnused + pAddress);
	} else if (pAddress <= 0xFFFF) {
		pAddress -= 0x4020;
		return *(m_pCartridge + pAddress);
	}
	return 0xFF;
}

void MMU::Write(uint16_t pAddress, uint8_t cVal) {
	if (!m_bInitialized) {
		return;
	}

	if (pAddress < 0x2000) { // Internal RAM
		// 0x0800 - 0x0FFF Mirrors 0x0000 - 0x007FF
		// 0x1000 - 0x07FF Mirrors 0x0000 - 0x007FF
		// 0x1800 - 0x1FFF Mirrors 0x0000 - 0x007FF
		pAddress %= 0x0800;
		*(m_pRAM + pAddress) = cVal;
	} else if (pAddress < 0x4000) {
		pAddress -= 0x2000;
		pAddress %= 0x0008;
		*(m_pPPURegs + pAddress) = cVal;
	} else if (pAddress < 0x4018) {
		pAddress -= 0x4000;
		*(m_pAPURegs + pAddress) = cVal;
	} else if (pAddress < 0x4020) {
		pAddress -= 0x4018;
		*(m_pAPURegsUnused + pAddress) = cVal;
	} else if (pAddress <= 0xFFFF) {
		pAddress -= 0x4020;
		*(m_pCartridge + pAddress) = cVal;
	}
}

uint16_t MMU::ReadAddress(uint16_t pAddress) {
	if (!m_bInitialized) {
		return 0xFFFF;
	}
	uint8_t lo = this->Read(pAddress);
	uint8_t hi = this->Read(pAddress + 1);
	
	return (static_cast<uint16_t>(hi) << 8) | lo;
}

void MMU::WriteAddress(uint16_t pAddress, uint16_t pVal) {
	if (!m_bInitialized) {
		return;
	}
	uint8_t lo = pVal & 0x00FF;
	uint8_t hi = (pVal & 0xFF00) >> 8;
	
	this->Write(pAddress, lo);
	this->Write(pAddress + 1, hi);
}

void MMU::Push(uint8_t* ppSp, uint8_t cVal) {
	if (!m_bInitialized) {
		return;
	}

	// Stack is 0x0100 - 0x01FF, stack pointer is  bit so we can offset into RAM
	// by 0x0100 and guarantee we stick to space in the stack
	*(m_pRAM + 0x0100 + (*ppSp)) = cVal;
	--(*ppSp);
}

uint8_t MMU::Pull(uint8_t* ppSp) {
	if (!m_bInitialized) {
		return 0xFF;
	}

	// Stack is 0x0100 - 0x01FF, stack pointer is  bit so we can offset into RAM
	// by 0x0100 and guarantee we stick to space in the stack
	++(*ppSp);
	return *(m_pRAM + 0x0100 + (*ppSp));
}

void MMU::PushAddress(uint8_t* ppSp, uint16_t pVal) {
	if (!m_bInitialized) {
		return;
	}
	uint8_t lo = pVal & 0x00FF;
	uint8_t hi = (pVal & 0xFF00) >> 8;

	this->Push(ppSp, hi);
	this->Push(ppSp, lo);
}

uint16_t MMU::PullAddress(uint8_t* ppSp) {
	if (!m_bInitialized) {
		return 0xFFFF;
	}

	uint8_t lo = this->Pull(ppSp);
	uint8_t hi = this->Pull(ppSp);

	return (static_cast<int16_t>(hi) << 8) | lo;
}

void MMU::LoadROM(const void* src, uintptr_t dst, size_t size) {
	for (uint32_t i = 0; i < size; i++) {
		this->Write(dst + i, *(reinterpret_cast<const uint8_t*>(src) + i));
	}
}
}
