#pragma once
namespace Intel_Hex
{
	constexpr auto bitsPerByte = 8;
	
	//Changes the character to a int value
	//if the char is 'A' the value is 10 in hex
	static inline uint8_t char_to_hex(uint8_t c);

	template<typename T>
	static inline T combine(T LSB, T MSB);
	{
		return (LSB | (MSB << (((sizeof(T) * bitsPerByte) / 2)));
	}
};