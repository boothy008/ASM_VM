#include "Hardware.h"

Hardware::~Hardware()
{
	delete[] m_pRegisters;
	delete[] m_pMemory;
}
