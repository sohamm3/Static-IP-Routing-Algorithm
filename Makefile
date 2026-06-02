# Makefile for the Static IP Routing project.
#
# Targets:
#   make            -> release build (alias for `make release`)
#   make release    -> optimized build  -> ./routing
#   make asan       -> AddressSanitizer + UndefinedBehaviorSanitizer build -> ./routing_asan
#   make clean      -> remove built binaries
#
# Note: the asan target requires a compiler/runtime that ships libasan/libubsan
# (gcc or clang on Linux/macOS). It is not expected to link on older MinGW.org
# toolchains on Windows.

CC      ?= gcc
CSTD    ?= -std=gnu11
WARN     = -Wall -Wextra -Wpedantic
SRC      = main.c functions.c
HDR      = header.h

RELEASE_BIN = routing
ASAN_BIN    = routing_asan

.PHONY: all release asan clean

all: release

release: $(SRC) $(HDR)
	$(CC) $(CSTD) $(WARN) -O2 $(SRC) -o $(RELEASE_BIN)

asan: $(SRC) $(HDR)
	$(CC) $(CSTD) $(WARN) -g -O1 -fno-omit-frame-pointer \
		-fsanitize=address,undefined $(SRC) -o $(ASAN_BIN)

clean:
	rm -f $(RELEASE_BIN) $(ASAN_BIN) $(RELEASE_BIN).exe $(ASAN_BIN).exe
