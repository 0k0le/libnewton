/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\
 *  \--\ macro.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <string.h>

#define PRINT(str, ...) printf(str "\n", ##__VA_ARGS__)

#ifdef _DEBUG
#define DEBUG(str, ...) printf("[DEBUG] " str "\n", ##__VA_ARGS__)
#define ERR(str, ...) fprintf(stderr, "[ERROR] " str "\n%s\n", ##__VA_ARGS__, strerror(errno))
#define WARN(str, ...) printf("[WARN] " str "\n", ##__VA_ARGS__)
#else
#define DEBUG(str, ...)
#define ERR(str, ...) fprintf(stderr, "[ERROR] " str "\n", ##__VA_ARGS__)
#define WARN(str, ...) printf("! " str " !\n", ##__VA_ARGS__)
#endif


#define FATAL(str, ...) {\
	ERR(str, ##__VA_ARGS__);\
	exit(EXIT_FAILURE);\
}

#define UNUSED(x)(void)(x)
