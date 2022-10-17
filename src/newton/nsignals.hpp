/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\
 *  \--\ nsignals.hpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\
 */

#pragma once

#include <vector>

#define MAX_SIGNAL_LEN 128
#define NSLOT(x)((nslot)(x))

typedef void (*nslot)(void *data);
typedef char * nsignal;
typedef const char * c_nsignal;

typedef struct npair {
	nslot slot;
	nsignal signal;
} NPAIR, *PNPAIR;

void nsignal_lock();
void nsignal_unlock();

void nconnect(c_nsignal sig, nslot slot);
void nemit(c_nsignal sig, void *data);

void ncleanup();
