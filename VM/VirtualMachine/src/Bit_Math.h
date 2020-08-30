#pragma once
namespace bit_math
{
	//Get value from data when the data is not stored together
	template<typename T>
	static T get_value_with_mask(T mask, T data);
};

template <typename T>
T bit_math::get_value_with_mask(T mask, T data)
{
	size_t maskPos = 0, valPos = 0;
	T value = 0;

	for(;maskPos < (sizeof(T) * 8); ++maskPos)
	{
		//If the mask bit is 1
		if(mask & (1 << maskPos))
		{
			//If data bit is 1 set value bit to 1
			if(data & (1 << maskPos))
			{
				value |= (1 << valPos);
			}
			
			valPos++;
		}
	}

	return value;
}