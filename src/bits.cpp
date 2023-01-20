/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ bits.cpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#include "newton/bits.hpp"
#include "newton/macro.hpp"

#include <string.h>

uint16_t Int16Mask(const uint16_t num, const char * const mask) {
	char reversed[17];
	uint16_t ret = num;

	if(strnlen(mask, 16) != 16)
		return -1;

	memset(reversed, 0, 17);
	for(int i = 0; i < 16; i++) {
		reversed[i] = mask[15-i];
	}

	DEBUG("Normal Mask: %s --> Reverse Mask: %s", mask, reversed);

	for(int i = 0; i < 16; i++) {
		if(reversed[i] == 'x')
			continue;
		else if(reversed[i] == '1')
			ret = BITON_MASK<<i | ret;
		else
			ret = ~(BITON_MASK<<i) & ret;
	}

	DEBUG("Original: 0x%x --> Masked: 0x%x", num, ret);

	return ret;
}

void BinaryPrint(const int num, const int bitsize) {
	for(int i = 0; i < bitsize; i++) {
		if(num & BITON_MASK<<((bitsize-1)-i))
			printf("1");
		else
			printf("0");
	}

	puts("");
}
