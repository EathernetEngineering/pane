#ifndef CEE_PANE_MMU_H_
#define CEE_PANE_MMU_H_

#include <cstdint>

namespace pane {
class MMU {
public:
	MMU();
	~MMU();

	void Init();
	void Shutdown();

	uint8_t Read(uint16_t pAddress);
	void Write(uint16_t pAddress, uint8_t cVal);

	uint16_t ReadAddress(uint16_t pAddress);
	void WriteAddress(uint16_t pAddress, uint16_t pVal);

	void Push(uint8_t* ppSp, uint8_t cVal);
	uint8_t Pull(uint8_t* ppSp);

	void PushAddress(uint8_t* ppSp, uint16_t pVal);
	uint16_t PullAddress(uint8_t* ppSp);

private:
	uint8_t* m_pRAM;
	uint8_t* m_pROMRAM;
	uint8_t* m_pROM;

	uint8_t* m_pPPURegs;
	uint8_t* m_pAPURegs;
	uint8_t* m_pAPURegsUnused;

	bool m_bInitialized;
};
}

#endif

