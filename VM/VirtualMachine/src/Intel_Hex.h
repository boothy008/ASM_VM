#pragma once
namespace Intel_Hex
{
	constexpr auto bitsPerByte = 8;
	constexpr unsigned char IntexHexEOF = 0x01;
	
	//Changes the character to a int value
	//if the char is 'A' the value returned is 10 in hex
	static inline uint8_t char_to_hex(uint8_t c);

	template<typename T>
	static inline T combine(T MSB, T LSB)
	{
		return (LSB | (MSB << (((sizeof(T) * bitsPerByte) / 2)));
	}

	template<typename T, typename R>
	static inline R extend(T MSB, T LSB)
	{
		return (static_cast<R>(LSB) | (MSB << sizeof(T)));
	}
};