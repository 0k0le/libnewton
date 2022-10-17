/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\
 *  \--\ heap.cpp \--\
 *   \\\\\\\\\\\\\\\\\\
 */

#include "newton/heap.hpp"

void *ec_malloc(size_t size) {
	void *mem = malloc(size);
	if(mem == nullptr)
		FATAL("Failed to allocate memory");

	return mem;
}

void *ec_malloc_zero(size_t size) {
	void *mem = ec_malloc(size);
	memset(mem, 0, size);

	return mem;
}
