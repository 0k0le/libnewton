/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\
 *  \--\ ethercat.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <stdint.h>

typedef struct EtherCATDatagramHeader {
	uint8_t 	cmd;
	uint8_t 	idx;
	uint32_t 	address;
	uint16_t	len : 11;
	uint8_t		R: 3;
	uint8_t 	C: 1;
	uint8_t 	M: 1;
	uint16_t	IRQ;
} EtherCATDatagramHeader;

typedef struct EtherCATFrameHeader {
	uint16_t len: 11;
	uint8_t reserved: 1;
	uint8_t type: 4;
} EtherCATFrameHeader;
