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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#include <stdint.h>
#include <windows.h>
#define usleep(val) Sleep(val/1000)
#define sleep(val) Sleep(val*1000)
#define WINEXPORT __declspec(dllexport)
#else
#define WINEXPORT
#endif

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <string.h>

#define PRINT(str, ...) printf(str "\n", ##__VA_ARGS__)

#ifdef _DEBUG
#define DEBUG(str, ...) printf("[DEBUG] " str "\n", ##__VA_ARGS__)
#define ERR(str, ...) { fprintf(stderr, "[ERROR] " str "\n%s\n", ##__VA_ARGS__, strerror(errno)); fflush(stderr); }
#define WARN(str, ...) printf("[WARN] " str "\n", ##__VA_ARGS__)
#else
#define DEBUG(str, ...)
#define ERR(str, ...) { fprintf(stderr, "[ERROR] " str "\n", ##__VA_ARGS__); fflush(stderr); }
#define WARN(str, ...) printf("! " str " !\n", ##__VA_ARGS__)
#endif


#define FATAL(str, ...) {\
	ERR(str, ##__VA_ARGS__);\
	exit(EXIT_FAILURE);\
}

#define UNUSED(x)(void)(x)
