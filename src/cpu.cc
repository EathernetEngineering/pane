#include "cpu.h"
#include "ram.h"

#include <cstring>
#include <stdexcept>
#include <format>

namespace nanjae {
CPU::CPU() {

}

CPU::~CPU() {
	m_pRAM.reset();
}

void CPU::SetRAM(std::shared_ptr<RAM> ram) {
	m_pRAM = ram;
}

void CPU::Start() {
	std::memset(reinterpret_cast<void*>(&m_xRegs), 0, sizeof(CPU::Registers));
	m_xRegs.pc = m_pRAM->Read(0xFFFC);

	m_bInturruptPending = false;
	m_eInterruptType = INT_NONE;

	m_xRegs.sr = 0x24;

	m_nCycles = m_nTotalCycles = 0;

	this->Execute();
}

void CPU::Execute() {
	m_nTotalCycles++;
	if (m_nCycles == 0) {
		if (m_eInterruptType != INT_NONE) {
			m_bInturruptPending = true;
			m_nCycles = 6;
			return;
		}
		m_nOpCode = m_pRAM->Read(m_xRegs.pc++);
		m_xInstruction = g_xInstructionLUT[m_nOpCode];
		this->GetOperandAddress();

		m_nCycles += g_nCyclesLUT[m_nOpCode];

		m_nCycles--;
		return;
	} else if (m_nCycles == 1) {
		m_nCycles--;
	} else {
		m_nCycles--;
		return;
	}

	if (m_bInturruptPending) {
		this->HandleInterrupt();
		m_bInturruptPending = false;
		return;
	}

	switch(m_xInstruction.oc) {
		case OC_ADC:
			ADC(m_pOperandAddress);
			break;
		
		case OC_AND:
			AND(m_pOperandAddress);
			break;
		
		case OC_ASL:
			m_xInstruction.am == AM_ACC ? ASLACC(m_pOperandAddress) : ASL(m_pOperandAddress);
			break;
		
		case OC_BCC:
			BCC(m_pOperandAddress);
			break;
		
		case OC_BCS:
			BCS(m_pOperandAddress);
			break;
		
		case OC_BEQ:
			BEQ(m_pOperandAddress);
			break;
		
		case OC_BIT:
			BIT(m_pOperandAddress);
			break;
		
		case OC_BMI:
			BMI(m_pOperandAddress);
			break;
		
		case OC_BNE:
			BNE(m_pOperandAddress);
			break;
		
		case OC_BPL:
			BPL(m_pOperandAddress);
			break;
		
		case OC_BRK:
			BRK();
			break;
		
		case OC_BVC:
			BVC(m_pOperandAddress);
			break;
		
		case OC_BVS:
			BVS(m_pOperandAddress);
			break;
		
		case OC_CLC:
			CLC();
			break;
		
		case OC_CLD:
			CLD();
			break;
		
		case OC_CLI:
			CLI();
			break;
		
		case OC_CLV:
			CLV();
			break;
		
		case OC_CMP:
			CMP(m_pOperandAddress);
			break;
		
		case OC_CPX:
			CPX(m_pOperandAddress);
			break;
		
		case OC_CPY:
			CPY(m_pOperandAddress);
			break;
		
		case OC_DEC:
			DEC(m_pOperandAddress);
			break;
		
		case OC_DEX:
			DEX();
			break;
		
		case OC_DEY:
			DEY();
			break;
		
		case OC_EOR:
			EOR(m_pOperandAddress);
			break;
		
		case OC_INC:
			INC(m_pOperandAddress);
			break;
		
		case OC_INX:
			INX();
			break;
		
		case OC_INY:
			INY();
			break;
		
		case OC_JMP:
			JMP(m_pOperandAddress);
			break;
		
		case OC_JSR:
			JSR(m_pOperandAddress);
			break;
		
		case OC_LDA:
			LDA(m_pOperandAddress);
			break;
		
		case OC_LDX:
			LDX(m_pOperandAddress);
			break;
		
		case OC_LDY:
			LDY(m_pOperandAddress);
			break;
		
		case OC_LSR:
			m_xInstruction.am == AM_ACC ? LSRACC(m_pOperandAddress) : LSR(m_pOperandAddress);
			break;
		
		case OC_NOP:
			NOP();
			break;
		
		case OC_ORA:
			ORA(m_pOperandAddress);
			break;
		
		case OC_PHA:
			PHA();
			break;
		
		case OC_PHP:
			PHP();
			break;
		
		case OC_PLA:
			PLA();
			break;
		
		case OC_PLP:
			PLP();
			break;
		
		case OC_ROL:
			m_xInstruction.am == AM_ACC ? ROLACC(m_pOperandAddress) : ROL(m_pOperandAddress);
			break;
		
		case OC_ROR:
			m_xInstruction.am == AM_ACC ? RORACC(m_pOperandAddress) : ROR(m_pOperandAddress);
			break;
		
		case OC_RTI:
			RTI(m_pOperandAddress);
			break;
		
		case OC_RTS:
			RTS(m_pOperandAddress);
			break;
		
		case OC_SBC:
			SBC(m_pOperandAddress);
			break;
		
		case OC_SEC:
			SEC();
			break;
		
		case OC_SED:
			SED();
			break;
		
		case OC_SEI:
			SEI();
			break;
		
		case OC_STA:
			STA(m_pOperandAddress);
			break;
		
		case OC_STX:
			STX(m_pOperandAddress);
			break;
		
		case OC_STY:
			STY(m_pOperandAddress);
			break;
		
		case OC_TAX:
			TAX();
			break;
		
		case OC_TAY:
			TAY();
			break;
		
		case OC_TSX:
			TSX();
			break;
		
		case OC_TXA:
			TXA();
			break;

		case OC_TXS:
			TXS();
			break;
		
		case OC_TYA:
			TYA();
			break;
		
		default:
			throw std::runtime_error(std::format("Invlad opcode {:#04x} called", static_cast<int8_t>(m_xInstruction.oc)));
	}
}
void CPU::Interrupt(InterruptType t) {
	m_bInturruptPending = true;
	m_eInterruptType = t;
}

void CPU::Reset() {
	m_xRegs.sr |= SR_INTERRUPT;
	m_xRegs.sp -= 3;
	m_xRegs.pc = m_pRAM->ReadWord(RESET_ADDRESS);
	m_nCycles = 0;
}

void CPU::ADC(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = m_xRegs.ac + opr + ((m_xRegs.sr & SR_CARRY) != 0);
	
	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetOverflowFlag(result, m_xRegs.ac, opr);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::AND(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int8_t result = m_xRegs.ac & opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::ASL(uint16_t oprAddr) {
	int16_t result = m_pRAM->Read(oprAddr) << 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_pRAM->Write(oprAddr, result & 0xFF);
}

void CPU::ASLACC(uint16_t oprAddr) {
	int16_t result = m_xRegs.ac << 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::BCC(uint16_t oprAddr) {
	if (!(m_xRegs.sr & SR_CARRY)) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BCS(uint16_t oprAddr) {
	if (m_xRegs.sr & SR_CARRY) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BEQ(uint16_t oprAddr) {
	if (m_xRegs.sr & SR_ZERO) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BIT(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int8_t result = m_xRegs.ac & opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	m_xRegs.sr |= (result == 0) ? SR_ZERO : 0;
	m_xRegs.sr |= opr & (SR_NEGATIVE | SR_OVERFLOW);
}

void CPU::BMI(uint16_t oprAddr) {
	if (m_xRegs.sr & SR_NEGATIVE) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BNE(uint16_t oprAddr) {
	if (!(m_xRegs.sr & SR_ZERO)) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BPL(uint16_t oprAddr) {
	if (!(m_xRegs.sr & SR_NEGATIVE)) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BRK() {
	m_xRegs.sp -= 2;
	m_pRAM->StackWriteWord(m_xRegs.sp, m_xRegs.pc + 2);
	m_xRegs.sr |= SR_INTERRUPT;
	m_xRegs.sp -= 1;
	m_pRAM->StackWrite(m_xRegs.sp, m_xRegs.sr);

	m_xRegs.pc = m_pRAM->ReadWord(IRQ_ADDRESS);
}

void CPU::BVC(uint16_t oprAddr) {
	if (!(m_xRegs.sr & SR_OVERFLOW)) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::BVS(uint16_t oprAddr) {
	if (m_xRegs.sr & SR_OVERFLOW) {
		m_xRegs.pc = oprAddr;
	}
}

void CPU::CLC() {
	m_xRegs.sr &= ~(SR_CARRY);
}

void CPU::CLD() {
	m_xRegs.sr &= ~(SR_DECIMAL);
}

void CPU::CLI() {
	m_xRegs.sr &= ~(SR_INTERRUPT);
}

void CPU::CLV() {
	m_xRegs.sr &= ~(SR_OVERFLOW);
}

void CPU::CMP(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = m_xRegs.ac - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::CPX(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = m_xRegs.x - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::CPY(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = m_xRegs.y - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::DEC(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = opr - 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_pRAM->Write(oprAddr, result);
}

void CPU::DEX() {
	int16_t result = m_xRegs.x - 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.x = result;
}

void CPU::DEY() {
	int16_t result = m_xRegs.y - 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.x = result;
}

void CPU::EOR(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int8_t result = opr ^ m_xRegs.ac;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::INC(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = opr + 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_pRAM->Write(oprAddr, result);
}

void CPU::INX() {
	int16_t result = m_xRegs.x + 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.x = result;
}

void CPU::INY() {
	int16_t result = m_xRegs.y + 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.x = result;
}

void CPU::JMP(uint16_t oprAddr) {
	m_xRegs.pc = m_pRAM->ReadWord(oprAddr);
}

void CPU::JSR(uint16_t oprAddr) {
	m_xRegs.sp -= 2;
	m_pRAM->StackWriteWord(m_xRegs.sp, m_xRegs.pc + 2);
	m_xRegs.pc = m_pRAM->ReadWord(oprAddr);
}

void CPU::LDA(uint16_t oprAddr) {
	m_xRegs.ac = m_pRAM->Read(oprAddr);
	CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::LDX(uint16_t oprAddr) {
	m_xRegs.x = m_pRAM->Read(oprAddr);
	CheckAndSetZNFlags(m_xRegs.x);
}

void CPU::LDY(uint16_t oprAddr) {
	m_xRegs.y = m_pRAM->Read(oprAddr);
	CheckAndSetZNFlags(m_xRegs.y);
}

void CPU::LSR(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = opr >> 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result & 1 ? 0xFFFF : 0x0000);

	m_pRAM->Write(oprAddr, result & 0xFF);
}

void CPU::LSRACC(uint16_t oprAddr) {
	int8_t opr = m_xRegs.ac;
	int16_t result = m_xRegs.ac >> 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result & 1 ? 0xFFFF : 0x0000);

	m_xRegs.ac = result & 0xFF;
}

void CPU::NOP() {
	;
}

void CPU::ORA(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int8_t result = m_xRegs.ac | opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::PHA() {
	m_xRegs.sp--;
	m_pRAM->Write(m_xRegs.sp, m_xRegs.ac);
}

void CPU::PHP() {
	m_xRegs.sp--;
	m_pRAM->Write(m_xRegs.sp, m_xRegs.sr | SR_INTERRUPT | 1 << 5);
}

void CPU::PLA() {
	m_xRegs.ac = m_pRAM->Read(m_xRegs.sp);
	m_xRegs.sp++;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::PLP() {
	m_xRegs.sr = m_pRAM->Read(m_xRegs.sp);
	m_xRegs.sp++;
}

void CPU::ROL(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = static_cast<int16_t>(opr) << 1;
	result |= (m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_pRAM->Write(oprAddr, result);
}

void CPU::ROLACC(uint16_t oprAddr) {
	int16_t result = oprAddr << 1;
	result |= (m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_xRegs.ac = static_cast<int8_t>(result);
}

void CPU::ROR(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = static_cast<int16_t>(opr) >> 1;
	result |= ((m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr) << 7;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(opr & 1 ? 0xFFFF : 0x0000);

	m_pRAM->Write(oprAddr, result);
}

void CPU::RORACC(uint16_t oprAddr) {
	int16_t result = oprAddr >> 1;
	result |= ((m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr) << 7;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(oprAddr & 1 ? 0xFFFF : 0x0000);

	m_xRegs.ac = result;
}

void CPU::RTI(uint16_t oprAddr) {
	m_xRegs.sr = m_pRAM->StackRead(m_xRegs.sp);
	m_xRegs.sp -= 1;
	m_xRegs.pc = m_pRAM->StackReadWord(m_xRegs.sp);
	m_xRegs.sp -= 2;
}

void CPU::RTS(uint16_t oprAddr) {
	m_xRegs.pc = m_pRAM->StackReadWord(m_xRegs.sp);
	m_xRegs.sp -= 2;
	m_xRegs.pc++;
}

void CPU::SBC(uint16_t oprAddr) {
	int8_t opr = m_pRAM->Read(oprAddr);
	int16_t result = m_xRegs.ac - opr - ((m_xRegs.sr & SR_CARRY) != 0);

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetOverflowFlag(result, m_xRegs.ac, opr);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::SEC() {
	m_xRegs.sr = SR_CARRY;
}

void CPU::SED() {
	m_xRegs.sr |= SR_DECIMAL;
}

void CPU::SEI() {
	m_xRegs.sr = SR_INTERRUPT;
}

void CPU::STA(uint16_t oprAddr) {
	m_pRAM->Write(oprAddr, m_xRegs.ac);
}

void CPU::STX(uint16_t oprAddr) {
	m_pRAM->Write(oprAddr, m_xRegs.x);
}

void CPU::STY(uint16_t oprAddr) {
	m_pRAM->Write(oprAddr, m_xRegs.y);
}

void CPU::TAX() {
	m_xRegs.x = m_xRegs.ac;
}

void CPU::TAY() {
	m_xRegs.y = m_xRegs.ac;
}

void CPU::TSX() {
	m_xRegs.x = m_xRegs.sp;
}

void CPU::TXA() {
	m_xRegs.ac = m_xRegs.x;
}

void CPU::TXS() {
	m_xRegs.sp = m_xRegs.x;
}

void CPU::TYA() {
	m_xRegs.ac = m_xRegs.y;
}

void CPU::GetOperandAddress() {
	switch (m_xInstruction.am) {
	case AM_ACC:
		m_pRAM->ReadWord(m_xRegs.pc); // Dummy read
		m_pOperandAddress = m_xRegs.ac;
		return;
	case AM_ABS:
		m_pOperandAddress = m_pRAM->ReadWord(m_xRegs.pc);
		m_xRegs.pc += 2;
		return;
	case AM_ABSX:
		m_pOperandAddress = m_pRAM->ReadWord(m_xRegs.pc);
		m_xRegs.pc += 2;
		switch (m_xInstruction.oc) {
		case OC_STA:
		case OC_ASL:
		case OC_DEC:
		case OC_INC:
		case OC_LSR:
		case OC_ROL:
		case OC_ROR:
				break;
		default:
			CheckIfPageBarrierCrossed(m_pOperandAddress, m_pOperandAddress + m_xRegs.x);
		}
		m_pOperandAddress += m_xRegs.x;
		return;
	case AM_ABSY:
		m_pOperandAddress = m_pRAM->ReadWord(m_xRegs.pc);
		m_xRegs.pc += 2;
		switch (m_xInstruction.oc) {
		case OC_STA:
		case OC_ASL:
		case OC_DEC:
		case OC_INC:
		case OC_LSR:
		case OC_ROL:
		case OC_ROR:
				break;
		default:
			CheckIfPageBarrierCrossed(m_pOperandAddress, m_pOperandAddress + m_xRegs.y);
		}
		m_pOperandAddress += m_xRegs.y;
		return;
	case AM_IMM:
		m_pOperandAddress = m_xRegs.pc;
		m_xRegs.pc += 1;
		return;
	case AM_IMPL:
		m_pOperandAddress = 0;;
		return;
	case AM_IND:
		m_pOperandAddress = m_pRAM->ReadWord(m_pRAM->ReadWord(m_xRegs.pc));
		m_xRegs.pc += 2;
		return;
	case AM_XIND:
		m_pOperandAddress = m_pRAM->ReadWord(m_pRAM->ReadWord(m_xRegs.pc) + m_xRegs.x);
		m_xRegs.pc += 2;
		return;
	case AM_INDY:
		m_pOperandAddress = m_pRAM->ReadWord(m_pRAM->ReadWord(m_xRegs.pc));
		m_xRegs.pc += 2;
		switch (m_xInstruction.oc) {
		case OC_STA:
		case OC_ASL:
		case OC_DEC:
		case OC_INC:
		case OC_LSR:
		case OC_ROL:
		case OC_ROR:
				break;
		default:
			CheckIfPageBarrierCrossed(m_pOperandAddress, m_pOperandAddress + m_xRegs.y);
		}
		m_pOperandAddress = m_pOperandAddress + m_xRegs.y;
		return;
	case AM_REL: {
			int16_t offset = m_pRAM->Read(m_xRegs.pc);
			if (offset & 0x80) {
				offset |= 0xFF00;
			}
			m_pOperandAddress = m_xRegs.pc + offset;
			m_xRegs.pc += 1;
			return;
		}
	case AM_ZPG:
		m_pOperandAddress = m_pRAM->Read(m_xRegs.pc);
		m_xRegs.pc += 1;
		return;
	case AM_ZPGX:
		m_pOperandAddress = m_pRAM->Read(m_xRegs.pc) + m_xRegs.x;
		m_xRegs.pc += 1;
		return;
	case AM_ZPGY:
		m_pOperandAddress = m_pRAM->Read(m_xRegs.pc) + m_xRegs.y;
		m_xRegs.pc += 1;
		return;
	default:
		throw std::runtime_error("Invalid address mode!");

	}
}

void CPU::CheckAndSetZNFlags(int8_t val) {
	m_xRegs.sr |= (val == 0) ? SR_ZERO : 0;
	m_xRegs.sr |= (val & 0x80) ? SR_NEGATIVE : 0;
}

void CPU::CheckAndSetOverflowFlag(int16_t result, int8_t lhs, int8_t rhs) {
	m_xRegs.sr |= ((result ^ lhs) & (result ^ rhs)) ? SR_OVERFLOW : 0;
}

void CPU::CheckAndSetCarryFlag(int16_t result) {
	m_xRegs.sr |= (result & 0xff00) ? SR_CARRY : 0;
}

void CPU::CheckIfPageBarrierCrossed(uint16_t pAddr1, uint16_t pAddr2) {
	if ((pAddr1 & 0xFF00) != (pAddr2 & 0xFF00)) {
		m_nCycles++;
	}
}

void CPU::HandleInterrupt() {
	int16_t addr;
	switch (m_eInterruptType) {
	case INT_IRQ:
		addr = IRQ_ADDRESS;
	case INT_NMI:
		addr = NMI_ADDRESS;
	case INT_RSI:
		addr = RESET_ADDRESS;
	case INT_SW:
		addr = IRQ_ADDRESS;
	case INT_NONE: // Fall through
	default:
		throw std::runtime_error("No interrupt type set or invalid type!");
	};

	m_eInterruptType = INT_NONE;

	m_xRegs.sp -= 2;
	m_pRAM->StackWriteWord(m_xRegs.sp, m_xRegs.pc);
	m_xRegs.sr |= SR_INTERRUPT;
	m_xRegs.sp -= 1;
	m_pRAM->StackWrite(m_xRegs.sp, m_xRegs.sr);
}
}

