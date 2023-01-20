#!/bin/bash

./build-submodules.bash

if [ "$1" == "DEBUG" ]; then
	make DEBUG="-g -D_DEBUG" -j4
else
	make -j4
fi
