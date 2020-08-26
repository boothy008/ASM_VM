#include "Intel_Hex.h"

uint8_t Intel_Hex::char_to_hex(uint8_t c)
{
	if (c < 'A')
		c -= '0';
	else
		c -= ('A' - 10);

	return c;
}
