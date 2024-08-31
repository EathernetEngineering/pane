#ifndef NANJAE_RAM_H_
#define NANJAE_RAM_H_

#include <cstdint>
#include <cstddef>

#define IRQ_ADDRESS 0xFFFE
#define NMI_ADDRESS 0xFFFA
#define RESET_ADDRESS 0xFFFC

namespace nanjae {
class RAM {
public:
	RAM();
	~RAM();

	int8_t Read(uint16_t uAddress);
	int16_t ReadWord(uint16_t uAddress);
	void Write(uint16_t uAddress, int8_t cVal);
	void WriteWord(uint16_t uAddress, int16_t iVal);

	int8_t StackRead(uint8_t uAddress);
	int16_t StackReadWord(uint8_t uAddress);
	void StackWrite(uint8_t uAddress, int8_t cVal);
	void StackWriteWord(uint8_t uAddress, int16_t iVal);

	void LoadROM(void* ROM, size_t size);
	void SerializeROMRAM(void* ROM, size_t size);

private:
	int8_t* m_pBuffer = nullptr;
};
}

#endif

