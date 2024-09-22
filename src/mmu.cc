#include "mmu.h"

#include <cstring>
#include <cstdlib>

namespace pane {
MMU::MMU()
 : m_pRAM(nullptr), m_pROMRAM(nullptr), m_pROM(nullptr), m_bInitialized(false)
{
}	

MMU::~MMU() {

}

void MMU::Init() {
	m_pRAM = reinterpret_cast<uint8_t*>(std::calloc(1, 0x0800));
	m_pROMRAM = reinterpret_cast<uint8_t*>(std::calloc(1, 0x2000));
	m_pROM = reinterpret_cast<uint8_t*>(std::calloc(1, 0x8000));
}

void MMU::Shutdown() {

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
		pAddress %= 0x0008;
		return *(m_pPPURegs + pAddress);
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
		pAddress %= 0x0008;

		*(m_pPPURegs + pAddress) = cVal;
	}
}

uint16_t MMU::ReadAddress(uint16_t pAddress) {
	if (!m_bInitialized) {
		return 0xFFFF;
	}
	uint8_t lo = this->Read(pAddress);
	uint8_t hi = this->Read(pAddress + 1);
	
	return (static_cast<uint16_t>(hi) << 8) & lo;
}

void MMU::WriteAddress(uint16_t pAddress, uint16_t pVal) {
	if (!m_bInitialized) {
		return;
	}
	uint8_t lo = pVal & 0x00FF;
	uint8_t hi = pVal & (0xFF00) >> 8;
	
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

	return (static_cast<int16_t>(hi) << 8) & lo;
}
}
