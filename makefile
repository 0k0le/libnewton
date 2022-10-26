# Matthew Todd Geiger <mgeiger@newtonlabs.com>
# makefile
#
# libnewton

CC=g++
BUILDOPTS=-Wall -Wextra -pedantic -fpic -c -O2 -g `pylon-config --cflags` `pkg-config opencv4 --cflags` -D_DEBUG
LDOPTS=`pylon-config --libs --libs-rpath` `pkg-config opencv4 --libs`

BINDIR=bin/
BIN=bin/libnewton.so

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

$(BIN): init $(nsignals.o) $(heap.o) $(basler-camera.o) $(server.o) $(net.o) $(client.o)
	$(CC) -shared $(nsignals.o) $(heap.o) $(basler-camera.o) $(server.o) $(net.o) $(client.o) $(LDOPTS) -o $(BIN)

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

init:
	mkdir -p bin/int

clean:
	rm -rf bin/
