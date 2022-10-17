/*
 * Matthew Todd Geiger <mgeiger@newtonlabs.com>
 * 
 * Newton Laboratories, libnewton
 *
 * \\\\\\\\\\\\\\\\\\\\\\
 *  \--\ nsignals.cpp \--\
 *   \\\\\\\\\\\\\\\\\\\\\\
 */

#include <cstring>
#include <thread>
#include <mutex>

#include "newton/nsignals.hpp"
#include "newton/heap.hpp"

static std::vector<NPAIR> _m_connected_pairs;
static std::mutex mtx;

void nconnect(c_nsignal signal, nslot slot) {
	NPAIR pair;

	// Copy signal to new pair
	pair.signal = (nsignal)ec_malloc(strnlen(signal, MAX_SIGNAL_LEN) + 2);
	strcpy(pair.signal, signal);

	// Copy slot to new pair
	pair.slot = slot;

	// Add pair to list
	_m_connected_pairs.push_back(pair);
}

void nemit(c_nsignal sig, void *data) {
	bool found = false;

	// Loop through pairs
	for(auto& i : _m_connected_pairs) {
		// Compare and launch
		if(strcmp(i.signal, sig) == 0) {
			found = true;
			i.slot(data);
		}
	}

	if(!found)
		fprintf(stderr, "Failed to emit %s\n", sig);
}

void ncleanup() {
	// Free heap strings
	for(auto& i : _m_connected_pairs) {
		free(i.signal);
	}

	// Clear vector
	if(!_m_connected_pairs.empty())
		_m_connected_pairs.erase(_m_connected_pairs.begin());
}

void nsignal_lock() {
	mtx.lock();
}

void nsignal_unlock() {
	mtx.unlock();
}
