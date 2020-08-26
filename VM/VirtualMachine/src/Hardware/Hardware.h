#pragma once
#include <cstdint>

class Hardware
{

protected:

	uint8_t* m_pRegisters;
	uint8_t* m_pMemory;

	uint8_t m_running;

public:
	virtual ~Hardware() = default;

	Hardware(size_t registerSize, size_t memorySize) : m_pRegisters(new uint8_t[registerSize]), m_pMemory(new uint8_t[memorySize]), m_running(1)
	{}

	//path to file programming hex
	virtual bool load_software(char* path) = 0;

	//Run loaded project
	virtual void run() = 0;
	
};

