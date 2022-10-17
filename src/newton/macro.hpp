/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, gtk-camera-app
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ macro.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <cstdio>
#include <cstdlib>

#define FATAL(str, ...) { \
	fprintf(stderr, str "\n", ##__VA_ARGS__); \
	exit(EXIT_FAILURE); \
}

#define UNUSED(x)(void)(x)
