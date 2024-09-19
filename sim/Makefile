.PHONY: all clean

PLATFORM   ?= PLATFORM_DESKTOP
BUILD_MODE ?= DEBUG
OS         ?= Unknown
THIS_FILE  ?= $(lastword $(MAKEFILE_LIST))

ifeq ($(OS),Windows_NT)
	OS = Windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		OS = Linux
	endif
	ifeq ($(UNAME),Darwin)
		OS = macOS
	endif
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(OS),Linux)
		CC = gcc
	endif
	ifeq ($(OS),macOS)
		CC = clang
	endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	CC = emcc
endif

ifeq ($(BUILD_MODE),DEBUG)
	ifeq ($(PLATFORM),PLATFORM_WEB)
		DEBUG_FLAG := -g0 -DDEBUG
	else
		DEBUG_FLAG := -g -DDEBUG
	endif
endif

ifeq ($(BUILD_MODE),RELEASE)
	EXTRA_FLAG := -O3
endif

all: sim

sim:
	@mkdir -p build
ifeq ($(PLATFORM),PLATFORM_WEB)
	@$(MAKE) -f $(THIS_FILE) web_build
else
ifeq ($(OS),Linux)
	@$(MAKE) -f $(THIS_FILE) linux_build
endif
ifeq ($(OS),macOS)
	@$(MAKE) -f $(THIS_FILE) macos_build
endif
endif

windows_build:
	@echo 'TODO: implement Windows build'

linux_build:
	$(CC) -o build/sim.o \
		-I include -L lib -lm \
		-Wall -Wextra -std=c11 $(DEBUG_FLAG) $(EXTRA_FLAG) \
		src/sim.c src/util.c lib/libraylib.a

macos_build:
	$(CC) -o build/sim.o \
		-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL \
		-I include -L lib -lm \
		-Wall -Wextra -std=c11 $(DEBUG_FLAG) $(EXTRA_FLAG) \
		src/sim.c src/util.c lib/libraylib.a

web_build:
	$(CC) -o build/index.html \
		-I include -L lib -lm \
		-Wall -Wextra -std=c11 $(DEBUG_FLAG) $(EXTRA_FLAG) \
		-DPLATFORM_WEB -s USE_GLFW=3 --shell-file src/minshell.html \
		src/sim.c src/util.c lib/libraylibweb.a

clean:
	rm build/*
