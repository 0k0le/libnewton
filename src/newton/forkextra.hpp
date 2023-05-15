/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\\
 *  \--\ forkextra.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <sys/types.h>
#include <unistd.h>

#include "macro.hpp"

template<typename T, typename... Args>
pid_t forkout(T func, Args... args) {
	pid_t pid = fork();
	if(pid == -1) {
		ERR("Failed to fork()");
		return -1;
	}

	if(pid == 0) {
		func(args...);
		exit(0);
	}

	return pid;
}

