/*
 * mylib.c
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#include "mylib.h"

sData ACK_START = {
	.DataArray = (uint8_t*)"start",
	.ui16_length = 5,
	.LengthArray = (uint8_t*)"00005",
};

sData ACK_DATA = {
	.DataArray = (uint8_t*)"data",
	.ui16_length = 4,
	.LengthArray = (uint8_t*)"00004",
};

sData ACK_END = {
	.DataArray = (uint8_t*)"endACK",
	.ui16_length = 6,
	.LengthArray = (uint8_t*)"00006",
};

sData ACK_ERROR = {
		.DataArray = (uint8_t*)"error",
		.ui16_length = 5,
		.LengthArray = (uint8_t*)"00005"
};

uint64_t Convert_Hex_to_Int(uint8_t *HexArray)
{
    uint64_t result = 0;
    result = strtol ((char*)HexArray, NULL, 16);
    return result;
}

uint64_t Convert_Hex_to_Int2(uint8_t *HexArray, uint8_t NumberHex)
{
    // Initializing base value to 1, i.e 16^0
    uint64_t base = 1;

    uint64_t dec_val = 0;

    // Extracting characters as digits from last
    // character
    for (int i = NumberHex - 1; i >= 0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value
        if ( *(HexArray + i) >= '0' && *(HexArray + i) <= '9')
        {
            dec_val = dec_val + (*(HexArray + i) - 48)* base;
            // incrementing base by power
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (*(HexArray + i) >= 'A' && *(HexArray + i) <= 'F')
        {
            dec_val = dec_val + ((*(HexArray + i) - 55) * base);
            // incrementing base by power
            base = base * 16;
        }
    }
    return dec_val;
}
