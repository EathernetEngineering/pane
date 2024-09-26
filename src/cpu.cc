#include "cpu.h"
#include "ram.h"

#include <cstring>
#include <stdexcept>
#include <format>

namespace pane {
CPU::CPU() {

}

CPU::~CPU() {
	m_pMMU.reset();
}

void CPU::SetMMU(std::shared_ptr<MMU> pMMU) {
	m_pMMU = pMMU;
}

void CPU::Start() {
	std::memset(reinterpret_cast<void*>(&m_xRegs), 0, sizeof(CPU::Registers));
	m_xRegs.pc = m_pMMU->ReadAddress(RESET_ADDRESS);

	m_bInturruptPending = false;
	m_eInterruptType = INT_NONE;

	m_xRegs.sr = 0x24;

	m_nCycles = m_nTotalCycles = 0;
}

void CPU::Execute() {
	m_nTotalCycles++;
	if (m_nCycles == 0) {
		if (m_eInterruptType != INT_NONE) {
			m_bInturruptPending = true;
			m_nCycles = 6;
			return;
		}
		m_nOpCode = m_pMMU->Read(m_xRegs.pc++);
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
			ADC();
			break;
		
		case OC_AND:
			AND();
			break;
		
		case OC_ASL:
			m_xInstruction.am == AM_ACC ? ASLACC() : ASL();
			break;
		
		case OC_BCC:
			BCC();
			break;
		
		case OC_BCS:
			BCS();
			break;
		
		case OC_BEQ:
			BEQ();
			break;
		
		case OC_BIT:
			BIT();
			break;
		
		case OC_BMI:
			BMI();
			break;
		
		case OC_BNE:
			BNE();
			break;
		
		case OC_BPL:
			BPL();
			break;
		
		case OC_BRK:
			BRK();
			break;
		
		case OC_BVC:
			BVC();
			break;
		
		case OC_BVS:
			BVS();
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
			CMP();
			break;
		
		case OC_CPX:
			CPX();
			break;
		
		case OC_CPY:
			CPY();
			break;
		
		case OC_DEC:
			DEC();
			break;
		
		case OC_DEX:
			DEX();
			break;
		
		case OC_DEY:
			DEY();
			break;
		
		case OC_EOR:
			EOR();
			break;
		
		case OC_INC:
			INC();
			break;
		
		case OC_INX:
			INX();
			break;
		
		case OC_INY:
			INY();
			break;
		
		case OC_JMP:
			JMP();
			break;
		
		case OC_JSR:
			JSR();
			break;
		
		case OC_LDA:
			LDA();
			break;
		
		case OC_LDX:
			LDX();
			break;
		
		case OC_LDY:
			LDY();
			break;
		
		case OC_LSR:
			m_xInstruction.am == AM_ACC ? LSRACC() : LSR();
			break;
		
		case OC_NOP:
			NOP();
			break;
		
		case OC_ORA:
			ORA();
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
			m_xInstruction.am == AM_ACC ? ROLACC() : ROL();
			break;
		
		case OC_ROR:
			m_xInstruction.am == AM_ACC ? RORACC() : ROR();
			break;
		
		case OC_RTI:
			RTI();
			break;
		
		case OC_RTS:
			RTS();
			break;
		
		case OC_SBC:
			SBC();
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
			STA();
			break;
		
		case OC_STX:
			STX();
			break;
		
		case OC_STY:
			STY();
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
	m_xRegs.pc = m_pMMU->ReadAddress(RESET_ADDRESS);
	m_nCycles = 0;
}

void CPU::ADC() {
	uint8_t opr = m_pMMU->Read(m_pOperandAddress);
	uint16_t result = m_xRegs.ac + opr + ((m_xRegs.sr & SR_CARRY) != 0);
	
	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetOverflowFlag(result, m_xRegs.ac, opr);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::AND() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int8_t result = m_xRegs.ac & opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::ASL() {
	int16_t result = static_cast<int16_t>(m_pMMU->Read(m_pOperandAddress)) << 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_pMMU->Write(m_pOperandAddress, result & 0xFF);
}

void CPU::ASLACC() {
	int16_t result = m_xRegs.ac << 1;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::BCC() {
	if (!(m_xRegs.sr & SR_CARRY)) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BCS() {
	if (m_xRegs.sr & SR_CARRY) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BEQ() {
	if (m_xRegs.sr & SR_ZERO) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BIT() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int8_t result = m_xRegs.ac & opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	m_xRegs.sr |= (result == 0) ? SR_ZERO : 0;
	m_xRegs.sr |= opr & (SR_NEGATIVE | SR_OVERFLOW);
}

void CPU::BMI() {
	if (m_xRegs.sr & SR_NEGATIVE) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BNE() {
	if (!(m_xRegs.sr & SR_ZERO)) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BPL() {
	if (!(m_xRegs.sr & SR_NEGATIVE)) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BRK() {
	m_pMMU->PushAddress(&m_xRegs.sp, m_xRegs.pc);
	m_xRegs.sr |= SR_INTERRUPT;
	m_pMMU->Push(&m_xRegs.sp, m_xRegs.sr);

	m_xRegs.pc = m_pMMU->ReadAddress(IRQ_ADDRESS);
}

void CPU::BVC() {
	if (!(m_xRegs.sr & SR_OVERFLOW)) {
		m_xRegs.pc = m_pOperandAddress;
	}
}

void CPU::BVS() {
	if (m_xRegs.sr & SR_OVERFLOW) {
		m_xRegs.pc = m_pOperandAddress;
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

void CPU::CMP() {
	uint8_t opr = m_pMMU->Read(m_pOperandAddress);
	uint16_t result = m_xRegs.ac - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::CPX() {
	uint8_t opr = m_pMMU->Read(m_pOperandAddress);
	uint16_t result = m_xRegs.x - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::CPY() {
	uint8_t opr = m_pMMU->Read(m_pOperandAddress);
	uint16_t result = m_xRegs.y - opr;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetZNFlags(result);
}

void CPU::DEC() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int16_t result = opr - 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_pMMU->Write(m_pOperandAddress, result);
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

void CPU::EOR() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int8_t result = opr ^ m_xRegs.ac;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::INC() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int16_t result = opr + 1;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_pMMU->Write(m_pOperandAddress, result);
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

void CPU::JMP() {
	m_xRegs.pc = m_pMMU->ReadAddress(m_pOperandAddress);
}

void CPU::JSR() {
	m_pMMU->PushAddress(&m_xRegs.sp, m_xRegs.pc);
	m_xRegs.pc = m_pMMU->ReadAddress(m_pOperandAddress);
}

void CPU::LDA() {
	m_xRegs.ac = m_pMMU->Read(m_pOperandAddress);
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::LDX() {
	m_xRegs.x = m_pMMU->Read(m_pOperandAddress);
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.x);
}

void CPU::LDY() {
	m_xRegs.y = m_pMMU->Read(m_pOperandAddress);
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.y);
}

void CPU::LSR() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int16_t result = opr >> 1;
	// Ensure bit shifted in is zero.
	result &= 0x7F;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(opr & 1 ? 0xFFFF : 0x0000);
	// Ensure negative flag is set to zero.
	m_xRegs.sr &= ~(SR_NEGATIVE);

	m_pMMU->Write(m_pOperandAddress, result & 0xFF);
}

void CPU::LSRACC() {
	int8_t opr = m_xRegs.ac;
	int16_t result = m_xRegs.ac >> 1;
	// Ensure bit shifted in is zero.
	result &= 0x7F;

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(opr & 1 ? 0xFFFF : 0x0000);
	// Ensure negative flag is set to zero.
	m_xRegs.sr &= ~(SR_NEGATIVE);

	m_xRegs.ac = result & 0xFF;
}

void CPU::NOP() {
	;
}

void CPU::ORA() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int8_t result = m_xRegs.ac | opr;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result;
}

void CPU::PHA() {
	m_pMMU->Push(&m_xRegs.sp, m_xRegs.ac);
}

void CPU::PHP() {
	m_pMMU->Push(&m_xRegs.sp, m_xRegs.sr | SR_INTERRUPT | 1 << 5);
}

void CPU::PLA() {
	m_xRegs.ac = m_pMMU->Pull(&m_xRegs.sp);

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::PLP() {
	m_xRegs.sr = m_pMMU->Pull(&m_xRegs.sp);
}

void CPU::ROL() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int16_t result = static_cast<int16_t>(opr) << 1;
	result |= (m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_pMMU->Write(m_pOperandAddress, result);
}

void CPU::ROLACC() {
	int16_t result = static_cast<int16_t>(m_pOperandAddress) << 1;
	result |= (m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(result);

	m_xRegs.ac = static_cast<int8_t>(result);
}

void CPU::ROR() {
	int8_t opr = m_pMMU->Read(m_pOperandAddress);
	int16_t result = static_cast<int16_t>(opr) >> 1;
	result |= ((m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr) << 7;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(opr & 1 ? 0xFFFF : 0x0000);

	m_pMMU->Write(m_pOperandAddress, result);
}

void CPU::RORACC() {
	int16_t result = static_cast<int16_t>(m_pOperandAddress) >> 1;
	result |= ((m_xRegs.sr ^ SR_CARRY) != m_xRegs.sr) << 7;

	this->CheckAndSetZNFlags(result);
	this->CheckAndSetCarryFlag(m_pOperandAddress & 1 ? 0xFFFF : 0x0000);

	m_xRegs.ac = result;
}

void CPU::RTI() {
	m_xRegs.sr = m_pMMU->Pull(&m_xRegs.sp);
	m_xRegs.pc = m_pMMU->PullAddress(&m_xRegs.sp);
}

void CPU::RTS() {
	m_xRegs.pc = m_pMMU->PullAddress(&m_xRegs.sp);
}

void CPU::SBC() {
	uint8_t opr = m_pMMU->Read(m_pOperandAddress);
	uint16_t result = m_xRegs.ac - opr - ((m_xRegs.sr & SR_CARRY) == 0);

	m_xRegs.sr &= ~(SR_CARRY | SR_ZERO | SR_NEGATIVE | SR_OVERFLOW);
	
	this->CheckAndSetCarryFlag(result);
	this->CheckAndSetOverflowFlag(result, m_xRegs.ac, ~opr);
	this->CheckAndSetZNFlags(result);

	m_xRegs.ac = result & 0xFF;
}

void CPU::SEC() {
	m_xRegs.sr |= SR_CARRY;
}

void CPU::SED() {
	m_xRegs.sr |= SR_DECIMAL;
}

void CPU::SEI() {
	m_xRegs.sr |= SR_INTERRUPT;
}

void CPU::STA() {
	m_pMMU->Write(m_pOperandAddress, m_xRegs.ac);
}

void CPU::STX() {
	m_pMMU->Write(m_pOperandAddress, m_xRegs.x);
}

void CPU::STY() {
	m_pMMU->Write(m_pOperandAddress, m_xRegs.y);
}

void CPU::TAX() {
	m_xRegs.x = m_xRegs.ac;
	
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.x);
}

void CPU::TAY() {
	m_xRegs.y = m_xRegs.ac;
	
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.y);
}

void CPU::TSX() {
	m_xRegs.x = m_xRegs.sp;
	
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.x);
}

void CPU::TXA() {
	m_xRegs.ac = m_xRegs.x;
	
	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::TXS() {
	m_xRegs.sp = m_xRegs.x;
}

void CPU::TYA() {
	m_xRegs.ac = m_xRegs.y;

	m_xRegs.sr &= ~(SR_ZERO | SR_NEGATIVE);
	this->CheckAndSetZNFlags(m_xRegs.ac);
}

void CPU::GetOperandAddress() {
	switch (m_xInstruction.am) {
	case AM_ACC:
		m_pMMU->ReadAddress(m_xRegs.pc); // Dummy read
		m_pOperandAddress = m_xRegs.ac;
		return;
	case AM_ABS:
		m_pOperandAddress = m_xRegs.pc;
		m_xRegs.pc += 2;
		return;
	case AM_ABSX:
		m_pOperandAddress = m_xRegs.pc;
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
		m_pOperandAddress = m_xRegs.pc;
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
		m_pOperandAddress = m_pMMU->ReadAddress(m_pMMU->ReadAddress(m_xRegs.pc));
		m_xRegs.pc += 2;
		return;
	case AM_XIND:
		m_pOperandAddress = m_pMMU->ReadAddress(m_pMMU->Read(m_xRegs.pc) + m_xRegs.x);
		m_xRegs.pc += 1;
		return;
	case AM_INDY:
		m_pOperandAddress = m_pMMU->ReadAddress(m_pMMU->Read(m_xRegs.pc));
		m_xRegs.pc += 1;
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
	case AM_REL: {
			int16_t offset = m_pMMU->Read(m_xRegs.pc);
			if (offset & 0x80) {
				offset |= 0xFF00;
			}
			m_xRegs.pc += 1;
			m_pOperandAddress = m_xRegs.pc + offset;
			return;
		}
	case AM_ZPG:
		m_pOperandAddress = m_pMMU->Read(m_xRegs.pc);
		m_xRegs.pc += 1;
		return;
	case AM_ZPGX:
		m_pOperandAddress = static_cast<uint8_t>(m_pMMU->Read(m_xRegs.pc) + m_xRegs.x);
		m_xRegs.pc += 1;
		return;
	case AM_ZPGY:
		m_pOperandAddress = static_cast<uint8_t>(m_pMMU->Read(m_xRegs.pc) + m_xRegs.y);
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

	m_pMMU->PushAddress(&m_xRegs.sp, m_xRegs.pc);
	m_xRegs.sr |= SR_INTERRUPT;
	m_pMMU->Push(&m_xRegs.sp, m_xRegs.sr);
}
}

