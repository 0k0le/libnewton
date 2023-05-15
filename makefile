# Matthew Todd Geiger <mgeiger@newtonlabs.com>
# makefile
#
# libnewton

DEBUG=

CC=g++
BUILDOPTS=-Wall -Wextra -pedantic -fpic  -c -O2 -g
LDOPTS=
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

net.o=$(INTDIR)/net.o
net.cpp=$(SRCDIR)/net.cpp

server.o=$(INTDIR)/server.o
server.cpp=$(SRCDIR)/server.cpp

client.o=$(INTDIR)/client.o
client.cpp=$(SRCDIR)/client.cpp

bits.o=$(INTDIR)/bits.o
bits.cpp=$(SRCDIR)/bits.cpp

$(BIN): init $(nsignals.o) $(heap.o) $(server.o) $(net.o) $(client.o) $(bits.o)
	$(CC) -fPIC -shared $(nsignals.o) $(heap.o) $(server.o) $(net.o) $(client.o) $(bits.o) $(LDOPTS) -o $(BIN)
	#ar rcs $(STATICBIN)	$(INTDIR)/*

$(bits.o): $(bits.cpp)
	$(CC) $(bits.cpp) $(BUILDOPTS) -o $(bits.o)

$(nsignals.o): $(nsignals.cpp)
	$(CC) $(nsignals.cpp) $(BUILDOPTS) -o $(nsignals.o)

$(heap.o): $(heap.cpp)
	$(CC) $(heap.cpp) $(BUILDOPTS) -o $(heap.o)

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
