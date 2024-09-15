.PHONY: all clean

PLATFORM   ?= PLATFORM_DESKTOP
BUILD_MODE ?= DEBUG

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	CC = gcc
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
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	$(CC) -o build/sim.o \
		-I include -L lib -lm -lpthread \
		-Wall -Wextra -std=c11 $(DEBUG_FLAG) $(EXTRA_FLAG) \
		src/sim.c src/util.c lib/libraylib.a
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	$(CC) -o build/index.html \
		-I include -L lib -lm \
		-Wall -Wextra -std=c11 $(DEBUG_FLAG) $(EXTRA_FLAG) \
		-DPLATFORM_WEB -s USE_GLFW=3 --shell-file src/minshell.html \
		src/sim.c src/util.c lib/libraylibweb.a
endif

clean:
	rm build/*
