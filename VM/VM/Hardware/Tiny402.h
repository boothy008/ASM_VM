#pragma once
#include <cstdint>
#include "IO_Tiny402.h"

class Tiny402
{
	//Registers
	enum
	{
		R_R0 = 0,
		R_R1,
		R_R2,
		R_R3,
		R_R4,
		R_R5,
		R_R6,
		R_R7,
		R_R8,
		R_R9,
		R_R10,
		R_R11,
		R_R12,
		R_R13,
		R_R14,
		R_R15,
		R_R16,
		R_R17,
		R_R18,
		R_R19,
		R_R20,
		R_R21,
		R_R22,
		R_R23,
		R_R24,
		R_R25,
		R_R26,
		R_R27,
		R_R28,
		R_R29,
		R_R30,
		R_R31,
		R_SIZE
	};

	//Instructions MSB 4 bits
	enum
	{
		I_0 = 0,
		I_1 = 1,
		I_2 = 2,
		I_CPI = 3,
		I_SBCI = 4,
		I_SUBI = 5,
		I_SBR = 6,
		I_CBR = 7,
		I_8 = 8,
		I_9 = 9,
		I_11 = 11,
		I_RJMP = 12,
		I_RCALL = 13,
		I_LDI = 14,
		I_15 = 15
	};

	uint8_t m_running;

	uint16_t m_PC;

	uint8_t* m_pRegisters;
	uint8_t* m_pMemory;
	
public:
	
	Tiny402() : m_running(1), m_PC(0), m_pRegisters(new uint8_t[R_SIZE]), m_pMemory(new uint8_t[MEMORY_SIZE])
	{}
};