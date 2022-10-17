/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ heap.hpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <stdlib.h>
#include <string.h>

#include "macro.hpp"

void *ec_malloc(size_t size);
void *ec_malloc_zero(size_t size);
