#include "Tiny402.h"

#include <algorithm>
#include <fstream>

uint8_t Tiny402::read_instruction()
{
	uint16_t fullInstruction = m_pMemory[CODE_START + m_PC];

	return 1;
}

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
				uint16_t dataSize = ((char_to_hex(buffer[0]) << 4) + char_to_hex(buffer[1]));

				softwareFile.read(buffer, 4);
				const uint16_t address = ((char_to_hex(buffer[0]) << 12) + (char_to_hex(buffer[1]) << 8) + (char_to_hex(buffer[2]) << 4) + char_to_hex(buffer[3]));

				softwareFile.read(buffer, 2);
				const uint8_t recordType = ((char_to_hex(buffer[0]) << 4) + char_to_hex(buffer[1]));

				if(recordType == 0x01)
				{
					returnVal = true;
					break;
				}

				auto memPtr = reinterpret_cast<uint16_t*>(m_pMemory + CODE_START + address);

				uint32_t checkVal = dataSize + address + recordType;

				while (dataSize > 0)
				{
					//Each instruction is 16 bits long
					softwareFile.read(buffer, 4);

					//Little_endian LSB is first
					const uint16_t instruction = ((char_to_hex(buffer[2]) << 12) | (char_to_hex(buffer[3]) << 8) | (char_to_hex(buffer[0]) << 4) | char_to_hex(buffer[1]));

					*memPtr++ = instruction;

					checkVal += instruction;

					dataSize -= 2;
				}

				//Checksum
				softwareFile.read(buffer, 2);

				const uint8_t checksum = (0xFF - static_cast<uint8_t>(checkVal)) + 1;
				uint8_t readVal = ((char_to_hex(buffer[0]) << 4) + char_to_hex(buffer[1]));
				
				if (checksum != readVal)
				{
					softwareFile.close();
					returnVal = false;
					break;
				}
			}
		}
	}

	return returnVal;
}

void Tiny402::run()
{
	m_running = 1;
	m_PC = 0;
	
	while (m_running)
	{
		//read next instruction
		m_running = read_instruction();
		m_PC++;
	}
}
