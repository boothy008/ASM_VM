#pragma once
#include "IO_Tiny402.h"
#include "Hardware.h"

class Tiny402 : public Hardware
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
	
	uint16_t m_PC;

	//===============================
	//Instructions
	//==============================

	
	
public:
	
	Tiny402() : Hardware(R_SIZE, MEMORY_SIZE),  m_PC(0)
	{}

	~Tiny402() = default;

	uint8_t read_instruction();
	
	//Loads program from Intel Hex file
	//:			- Start
	//04		- Number of bytes
	//0030		- Memory address
	//00		- Record type - 00 Data - 01 EOF - 02 Extended segment address - 03 Start Segment Address - 04 Extended Linear Address - 05 Start Linear Address
	//02337A41	- DATA - Tiny data is 16 bits wide using Little-endian so 02 33 
	//DC		- Checksum - (FF - (LSB of (04 + 00 + 30 + 00 + 02 + 33 + 7A + 41)) + 1) = DC
	//: 04 0030 00 02337A DC - Without spaces
	//https://en.wikipedia.org/wiki/Intel_HEX
	bool load_software(char* path) override;
	
	void run() override;
	
};