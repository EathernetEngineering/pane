#include "ram.h"

#include <cstdlib>
#include <new>

namespace pane {
RAM::RAM() {
	m_pBuffer = reinterpret_cast<int8_t*>(calloc(1, 0xFFFF)); // allocate 16K, maximum memory for the 6502
	if (m_pBuffer == nullptr) {
		throw std::bad_alloc();
	}
}

RAM::~RAM() {
	if (m_pBuffer != nullptr) {
		free(m_pBuffer);
	}
}

int8_t RAM::Read(uint16_t uAddress) {
	return *(m_pBuffer + uAddress);
}

int16_t RAM::ReadWord(uint16_t uAddress) {
	return *(m_pBuffer + uAddress);
}

void RAM::Write(uint16_t uAddress, int8_t cVal) {
	*(m_pBuffer + uAddress) = cVal;
}

void RAM::WriteWord(uint16_t uAddress, int16_t iVal) {
	*(m_pBuffer + uAddress) = iVal;
}

int8_t RAM::StackRead(uint8_t uAddress) {
	return *(m_pBuffer + uAddress + 0x0100);
}

int16_t RAM::StackReadWord(uint8_t uAddress) {
	return *(m_pBuffer + uAddress + 0x0100);
}

void RAM::StackWrite(uint8_t uAddress, int8_t cVal) {
	*(m_pBuffer + uAddress + 0x0100) = cVal;
}

void RAM::StackWriteWord(uint8_t uAddress, int16_t iVal) {
	*(m_pBuffer + uAddress + 0x0100) = iVal;
}
}

