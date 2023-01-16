#!/bin/bash

cd 3rd/SOEM/ && mkdir -p build && cd build && cmake .. && make -j4 && cd ../../../
