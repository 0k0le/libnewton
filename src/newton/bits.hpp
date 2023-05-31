/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ bits.hpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#pragma once

#include "macro.hpp"

#ifdef _WIN32
#define uint16_t unsigned short
#define uint32_t unsigned int
#else
#include <bits/stdint-uintn.h>
#endif

#define BITON_MASK 0b1
#define BITOFF_MASK 0b0

WINEXPORT uint16_t Int16Mask(const uint16_t num, const char * const mask);
WINEXPORT void BinaryPrint(const int num, const int bitsize);
