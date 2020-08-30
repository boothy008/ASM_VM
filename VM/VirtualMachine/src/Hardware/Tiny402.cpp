#include "Tiny402.h"
#include "Bit_Math.h"

#include <algorithm>
#include <fstream>
#include <Intel_Hex.h>

//Reg points to data address
void Tiny402::ldd(const uint16_t* reg, const uint16_t instr)
{
	//Mask for displacement
	const uint16_t mask = 0x2C03;
	const uint16_t displacement = bit_math::get_value_with_mask(mask, instr);

	//get destination register 0-31
	uint16_t dReg = ((instr & 0x01F0) >> 4);

	//Get a pointer to 
	auto regPtr = reinterpret_cast<uint16_t*>((*reg) + displacement);
}

Tiny402::~Tiny402() = default;

//Programs microcontroler memory
bool Tiny402::load_software(char* path)
{
	bool returnVal = false;
	std::ifstream softwareFile(path, std::ifstream::binary);

	if(softwareFile)
	{
		char* buffer = new char[4];
		
		while(!softwareFile.eof())
		{
			softwareFile.read(buffer, 1);

			//Check first bit
			if (buffer[0] == ':')
			{
				softwareFile.read(buffer, 2);
				uint8_t dataSize = Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[0]), Intel_Hex::char_to_hex(buffer[1])); // ((Intel_Hex::char_to_hex(buffer[0]) << 4) + Intel_Hex::char_to_hex(buffer[1]));

				softwareFile.read(buffer, 4);
				const uint16_t address = Intel_Hex::extend<uint8_t, uint16_t>(Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[0]), Intel_Hex::char_to_hex(buffer[1])),
					Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[2]), Intel_Hex::char_to_hex(buffer[3])));
				//((Intel_Hex::char_to_hex(buffer[0]) << 12) + (Intel_Hex::char_to_hex(buffer[1]) << 8) + (Intel_Hex::char_to_hex(buffer[2]) << 4) + Intel_Hex::char_to_hex(buffer[3]));

				softwareFile.read(buffer, 2);
				const uint8_t recordType = Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[0]), Intel_Hex::char_to_hex(buffer[1])); //((Intel_Hex::char_to_hex(buffer[0]) << 4) + Intel_Hex::char_to_hex(buffer[1]));

				if(recordType == Intel_Hex::IntexHexEOF)
				{
					returnVal = true;
					break;
				}

				//Pointer to the memory address to write the program
				auto memPtr = reinterpret_cast<uint16_t*>(m_pMemory + CODE_START + address);

				uint32_t checkVal = dataSize + address + recordType;

				while (dataSize > 0)
				{
					//Each instruction is 16 bits long
					softwareFile.read(buffer, 4);

					//Little_endian LSB is first
					const uint16_t instruction = Intel_Hex::extend<uint8_t, uint16_t>(Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[2]), Intel_Hex::char_to_hex(buffer[3])),
						Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[0]), Intel_Hex::char_to_hex(buffer[1])));
						//((Intel_Hex::char_to_hex(buffer[2]) << 12) | (Intel_Hex::char_to_hex(buffer[3]) << 8) | (Intel_Hex::char_to_hex(buffer[0]) << 4) | Intel_Hex::char_to_hex(buffer[1]));

					*memPtr++ = instruction;
					checkVal += instruction;
					dataSize -= 2;
				}

				//Checksum
				softwareFile.read(buffer, 2);

				const uint8_t checksum = (0xFF - static_cast<uint8_t>(checkVal)) + 1;
				const uint8_t readVal = Intel_Hex::combine(Intel_Hex::char_to_hex(buffer[0]), Intel_Hex::char_to_hex(buffer[1]));
					//((Intel_Hex::char_to_hex(buffer[0]) << 4) + Intel_Hex::char_to_hex(buffer[1]));
				
				if (checksum != readVal)
				{
					returnVal = false;
					break;
				}
			}
		}
		
		softwareFile.close();
	}

	return returnVal;
}

void Tiny402::run()
{
	m_running = 1;
	m_pc = 0;

	const auto* codeStart = reinterpret_cast<uint16_t*>(m_pMemory + CODE_START);

	//Fetch the first instruction
	uint16_t instr = *(codeStart + m_pc);
	
	while (m_running)
	{
		


		
		m_pc++;
		
		//read next instruction
		instr = *(codeStart + m_pc);
	}
}
