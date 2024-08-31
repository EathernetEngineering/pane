#ifndef NANJAE_CPU_H_
#define NANJAE_CPU_H_

#include <memory>
#include <unistd.h>

#include "ram.h"

namespace nanjae {

enum ProcessorFlags {
	SR_NONE      = 0,
	SR_CARRY     = 1 << 0,
	SR_ZERO      = 1 << 1,
	SR_INTERRUPT = 1 << 2,
	SR_DECIMAL   = 1 << 3,
	SR_BREAK     = 1 << 4,
	SR_OVERFLOW  = 1 << 6,
	SR_NEGATIVE  = 1 << 7
};

enum InterruptType {
	INT_NONE = 0,
	INT_IRQ,
	INT_NMI,
	INT_RSI,
	INT_SW
};

enum AddressMode {
	AM_NONE = 0,
	AM_ACC,
	AM_ABS,
	AM_ABSX,
	AM_ABSY,
	AM_IMM,
	AM_IMPL,
	AM_IND,
	AM_XIND,
	AM_INDY,
	AM_REL,
	AM_ZPG,
	AM_ZPGX,
	AM_ZPGY
};

enum OpCode {
	OC_NONE = 0,
	OC_ADC,
	OC_AND,
	OC_ASL,
	OC_BCC,
	OC_BCS,
	OC_BEQ,
	OC_BIT,
	OC_BMI,
	OC_BNE,
	OC_BPL,
	OC_BRK,
	OC_BVC,
	OC_BVS,
	OC_CLC,
	OC_CLD,
	OC_CLI,
	OC_CLV,
	OC_CMP,
	OC_CPX,
	OC_CPY,
	OC_DEC,
	OC_DEX,
	OC_DEY,
	OC_EOR,
	OC_INC,
	OC_INX,
	OC_INY,
	OC_JMP,
	OC_JSR,
	OC_LDA,
	OC_LDX,
	OC_LDY,
	OC_LSR,
	OC_NOP,
	OC_ORA,
	OC_PHA,
	OC_PHP,
	OC_PLA,
	OC_PLP,
	OC_ROL,
	OC_ROR,
	OC_RTI,
	OC_RTS,
	OC_SBC,
	OC_SEC,
	OC_SED,
	OC_SEI,
	OC_STA,
	OC_STX,
	OC_STY,
	OC_TAX,
	OC_TAY,
	OC_TSX,
	OC_TXA,
	OC_TXS,
	OC_TYA,
};

struct Instruction {
	OpCode oc;
	AddressMode am;
};

static const Instruction g_xInstructionLUT[256] = {
	          /*       0x0                   0x1                   0x2                   0x3                   0x4                   0x5                   0x6                   0x7                   0x8                   0x9                   0xA                   0xB                   0xC                   0xD                   0xE                   0xF      */
	/* 0x0 */ { OC_BRK,  AM_IMPL }, { OC_ORA,  AM_IND  }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ORA,  AM_ZPG  }, { OC_ASL,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_PHP,  AM_IMPL }, { OC_ORA,  AM_IMM  }, { OC_ASL,  AM_ACC  }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ORA,  AM_ABS  }, { OC_ASL,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0x1 */ { OC_BPL,  AM_REL  }, { OC_ORA,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ORA,  AM_ZPGX }, { OC_ASL,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_CLC,  AM_IMPL }, { OC_ORA,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ORA,  AM_ABSX }, { OC_ASL,  AM_ABSX }, { OC_NONE, AM_NONE },
	/* 0x2 */ { OC_JSR,  AM_ABS  }, { OC_AND,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_BIT , AM_ZPG  }, { OC_AND,  AM_ZPG  }, { OC_ROL,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_PLP,  AM_IMPL }, { OC_AND,  AM_IMM  }, { OC_ROL,  AM_ACC  }, { OC_NONE, AM_NONE }, { OC_BIT,  AM_ABS  }, { OC_AND,  AM_ABS  }, { OC_ROL,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0x3 */ { OC_BMI,  AM_REL  }, { OC_AND,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_AND,  AM_ZPGX }, { OC_ROL,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_SEC,  AM_IMPL }, { OC_AND,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_AND,  AM_ABSX }, { OC_ROL,  AM_ABSX }, { OC_NONE, AM_NONE },
	/* 0x4 */ { OC_RTI,  AM_IMPL }, { OC_EOR,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_EOR,  AM_ZPG  }, { OC_LSR,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_PHA,  AM_IMPL }, { OC_EOR,  AM_IMM  }, { OC_LSR,  AM_ACC  }, { OC_NONE, AM_NONE }, { OC_JMP,  AM_ABS  }, { OC_EOR,  AM_ABS  }, { OC_LSR,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0x5 */ { OC_BVC,  AM_REL  }, { OC_EOR,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_EOR,  AM_ZPGX }, { OC_LSR,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_CLI,  AM_IMPL }, { OC_EOR,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_EOR,  AM_ABSX }, { OC_LSR,  AM_ABSX }, { OC_NONE, AM_NONE },
	/* 0x6 */ { OC_RTS,  AM_IMPL }, { OC_ADC,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ADC,  AM_ZPG  }, { OC_ROR,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_PLA,  AM_IMPL }, { OC_ADC,  AM_IMM  }, { OC_ROR,  AM_ACC  }, { OC_NONE, AM_NONE }, { OC_JMP,  AM_IND  }, { OC_ADC,  AM_ABS  }, { OC_ROR,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0x7 */ { OC_BVS,  AM_REL  }, { OC_ADC,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ADC,  AM_ZPGX }, { OC_ROR,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_SEI,  AM_IMPL }, { OC_ADC,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_ADC,  AM_ABSX }, { OC_ROR,  AM_ABSX }, { OC_NONE, AM_NONE },
	/* 0x8 */ { OC_NONE, AM_NONE }, { OC_STA,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_STY,  AM_ZPG  }, { OC_STA,  AM_ZPG  }, { OC_STX,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_DEY,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_TXA,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_STY,  AM_ABS  }, { OC_STA,  AM_ABS  }, { OC_STX,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0x9 */ { OC_BCC,  AM_REL  }, { OC_STA,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_STY,  AM_ZPGX }, { OC_STA,  AM_ZPGX }, { OC_STX,  AM_ZPGY }, { OC_NONE, AM_NONE }, { OC_TYA,  AM_IMPL }, { OC_STA,  AM_ABSY }, { OC_TXS,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_STA,  AM_ABSX }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE },
	/* 0xA */ { OC_LDY,  AM_IMM  }, { OC_LDA,  AM_XIND }, { OC_LDX,  AM_IMM  }, { OC_NONE, AM_NONE }, { OC_LDY,  AM_ZPG  }, { OC_LDA,  AM_ZPG  }, { OC_LDX,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_TAY,  AM_IMPL }, { OC_LDA,  AM_IMM  }, { OC_TAX,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_LDY,  AM_ABS  }, { OC_LDA,  AM_ABS  }, { OC_LDX,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0xB */ { OC_BCS,  AM_REL  }, { OC_LDA,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_LDY,  AM_ZPGX }, { OC_LDA,  AM_ZPGX }, { OC_LDX,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_CLV,  AM_IMPL }, { OC_LDA,  AM_ABSY }, { OC_TSX,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_LDY,  AM_ABSX }, { OC_LDA,  AM_ABSX }, { OC_LDX,  AM_ABSY }, { OC_NONE, AM_NONE },
	/* 0xC */ { OC_CPY,  AM_IMM  }, { OC_CMP,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_CPY,  AM_ZPG  }, { OC_CMP,  AM_ZPG  }, { OC_DEC,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_INY,  AM_IMPL }, { OC_CMP,  AM_IMM  }, { OC_DEX,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_CPY,  AM_ABS  }, { OC_CMP,  AM_ABS  }, { OC_DEC,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0xD */ { OC_BNE,  AM_REL  }, { OC_CMP,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_CMP,  AM_ZPGX }, { OC_DEC,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_CLD,  AM_IMPL }, { OC_CMP,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_CMP,  AM_ABSX }, { OC_DEC,  AM_ABSX }, { OC_NONE, AM_NONE },
	/* 0xE */ { OC_CPX,  AM_IMM  }, { OC_SBC,  AM_XIND }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_CPX,  AM_ZPG  }, { OC_SBC,  AM_ZPG  }, { OC_INC,  AM_ZPG  }, { OC_NONE, AM_NONE }, { OC_INX,  AM_IMPL }, { OC_SBC,  AM_IMM  }, { OC_NOP,  AM_IMPL }, { OC_NONE, AM_NONE }, { OC_CPX,  AM_ABS  }, { OC_SBC,  AM_ABS  }, { OC_INC,  AM_ABS  }, { OC_NONE, AM_NONE },
	/* 0xF */ { OC_BEQ,  AM_REL  }, { OC_SBC,  AM_INDY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_SBC,  AM_ZPGX }, { OC_INC,  AM_ZPGX }, { OC_NONE, AM_NONE }, { OC_SED,  AM_IMPL }, { OC_SBC,  AM_ABSY }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_NONE, AM_NONE }, { OC_SBC,  AM_ABSX }, { OC_INC,  AM_ABSX }, { OC_NONE, AM_NONE },
};

static const int32_t g_nCyclesLUT[256] {
	/*       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
	/* 0 */  7, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
	/* 1 */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	/* 2 */  6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
	/* 3 */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	/* 4 */  6, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
	/* 5 */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	/* 6 */  6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
	/* 7 */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	/* 8 */  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
	/* 9 */  2, 6, 0, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
	/* A */  2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
	/* B */  2, 5, 0, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
	/* C */  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
	/* D */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
	/* E */  2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
	/* F */  2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7
};

class CPU {
public:
	CPU();
	~CPU();

	void SetRAM(std::shared_ptr<RAM> ram);

	void Start();
	void Execute();

	void Interrupt(InterruptType t);
	void Reset();

private:
	void ADC(uint16_t oprAddr);
	void AND(uint16_t oprAddr);
	void ASL(uint16_t oprAddr);
	void ASLACC(uint16_t oprAddr);
	void BCC(uint16_t oprAddr);
	void BCS(uint16_t oprAddr);
	void BEQ(uint16_t oprAddr);
	void BIT(uint16_t oprAddr);
	void BMI(uint16_t oprAddr);
	void BNE(uint16_t oprAddr);
	void BPL(uint16_t oprAddr);
	void BRK();
	void BVC(uint16_t oprAddr);
	void BVS(uint16_t oprAddr);
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void CMP(uint16_t oprAddr);
	void CPX(uint16_t oprAddr);
	void CPY(uint16_t oprAddr);
	void DEC(uint16_t oprAddr);
	void DEX();
	void DEY();
	void EOR(uint16_t oprAddr);
	void INC(uint16_t oprAddr);
	void INX();
	void INY();
	void JMP(uint16_t oprAddr);
	void JSR(uint16_t oprAddr);
	void LDA(uint16_t oprAddr);
	void LDX(uint16_t oprAddr);
	void LDY(uint16_t oprAddr);
	void LSR(uint16_t oprAddr);
	void LSRACC(uint16_t oprAddr);
	void NOP();
	void ORA(uint16_t oprAddr);
	void PHA();
	void PHP();
	void PLA();
	void PLP();
	void ROL(uint16_t oprAddr);
	void ROLACC(uint16_t oprAddr);
	void ROR(uint16_t oprAddr);
	void RORACC(uint16_t oprAddr);
	void RTI(uint16_t oprAddr);
	void RTS(uint16_t oprAddr);
	void SBC(uint16_t oprAddr);
	void SEC();
	void SED();
	void SEI();
	void STA(uint16_t oprAddr);
	void STX(uint16_t oprAddr);
	void STY(uint16_t oprAddr);
	void TAX();
	void TAY();
	void TSX();
	void TXA();
	void TXS();
	void TYA();

	void GetOperandAddress();
	void CheckAndSetZNFlags(int8_t val);
	void CheckAndSetOverflowFlag(int16_t result, int8_t lhs, int8_t rhs);
	void CheckAndSetCarryFlag(int16_t result);
	void CheckIfPageBarrierCrossed(uint16_t pAddr, uint16_t pAddr2);

	void HandleInterrupt();

private:
	// Hardaware
	struct Registers {
		uint16_t pc;
		int8_t ac, y, x;
		int8_t sr, sp;
	} m_xRegs;

	std::shared_ptr<RAM> m_pRAM;

	// Timing
	uint32_t m_nCycles;
	uint32_t m_nTotalCycles;

	// Interrupt
	bool m_bInturruptPending;
	InterruptType m_eInterruptType;

	// State
	int8_t m_nOpCode;
	Instruction m_xInstruction;
	int16_t m_pOperandAddress;
};
}

#endif

