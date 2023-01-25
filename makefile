# Matthew Todd Geiger <mgeiger@newtonlabs.com>
# makefile
#
# libnewton

DEBUG=

CC=g++
BUILDOPTS=-Wall -Wextra -pedantic -fpic  -c -O2 -g `pylon-config --cflags` `pkg-config opencv4 --cflags` $(DEBUG) -I3rd/SOEM -I3rd/SOEM/osal -I3rd/SOEM/osal/linux -I3rd/SOEM/oshw -I3rd/SOEM/oshw/linux -lgclib -lgclibo
LDOPTS=`pylon-config --libs --libs-rpath` `pkg-config opencv4 --libs`
LDSTATICOPTS=

BINDIR=bin/
BIN=bin/libnewton.so
STATICBIN=bin/libnewton.a

INTDIR=bin/int
SRCDIR=src

nsignals.o=$(INTDIR)/nsignals.o
nsignals.cpp=$(SRCDIR)/nsignals.cpp

heap.o=$(INTDIR)/heap.o
heap.cpp=$(SRCDIR)/heap.cpp

basler-camera.o=$(INTDIR)/basler-camera.o
basler-camera.cpp=$(SRCDIR)/basler-camera.cpp

net.o=$(INTDIR)/net.o
net.cpp=$(SRCDIR)/net.cpp

server.o=$(INTDIR)/server.o
server.cpp=$(SRCDIR)/server.cpp

client.o=$(INTDIR)/client.o
client.cpp=$(SRCDIR)/client.cpp

maxon.o=$(INTDIR)/maxon.o
maxon.cpp=$(SRCDIR)/maxon.cpp

ethercat.o=$(INTDIR)/ethercat.o
ethercat.cpp=$(SRCDIR)/ethercat.cpp

bits.o=$(INTDIR)/bits.o
bits.cpp=$(SRCDIR)/bits.cpp

galil.o=$(INTDIR)/galil.o
galil.cpp=$(SRCDIR)/galil.cpp

$(BIN): init $(nsignals.o) $(heap.o) $(basler-camera.o) $(server.o) $(net.o) $(client.o) $(maxon.o) $(ethercat.o) $(bits.o) $(galil.o)
	$(CC) -fPIC -shared $(nsignals.o) $(heap.o) $(basler-camera.o) $(server.o) $(net.o) $(client.o) $(maxon.o) $(ethercat.o) $(bits.o) $(galil.o) 3rd/SOEM/build/libsoem.a $(LDOPTS) -o $(BIN)
	#ar rcs $(STATICBIN)	$(INTDIR)/*

$(bits.o): $(bits.cpp)
	$(CC) $(bits.cpp) $(BUILDOPTS) -o $(bits.o)

$(ethercat.o): $(ethercat.cpp)
	$(CC) $(ethercat.cpp) $(BUILDOPTS) -o $(ethercat.o)

$(maxon.o): $(maxon.cpp)
	$(CC) $(maxon.cpp) $(BUILDOPTS) -o $(maxon.o)

$(nsignals.o): $(nsignals.cpp)
	$(CC) $(nsignals.cpp) $(BUILDOPTS) -o $(nsignals.o)

$(heap.o): $(heap.cpp)
	$(CC) $(heap.cpp) $(BUILDOPTS) -o $(heap.o)

$(basler-camera.o): $(basler-camera.cpp)
	$(CC) $(basler-camera.cpp) $(BUILDOPTS) -o $(basler-camera.o)

$(net.o): $(net.cpp)
	$(CC) $(net.cpp) $(BUILDOPTS) -o $(net.o)

$(server.o): $(server.cpp)
	$(CC) $(server.cpp) $(BUILDOPTS) -o $(server.o)

$(client.o): $(client.cpp)
	$(CC) $(client.cpp) $(BUILDOPTS) -o $(client.o)

$(galil.o): $(galil.cpp)
	$(CC) $(galil.cpp) $(BUILDOPTS) -o $(galil.o)

init:
	mkdir -p bin/int

clean:
	rm -rf bin/
