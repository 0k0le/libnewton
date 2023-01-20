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

#include <bits/stdint-uintn.h>
#include <sys/types.h>
#include <unistd.h>

#define BITON_MASK 0b1
#define BITOFF_MASK 0b0

uint16_t Int16Mask(const uint16_t num, const char * const mask);
void BinaryPrint(const int num, const int bitsize);
