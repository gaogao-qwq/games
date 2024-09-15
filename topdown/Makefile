.PHONY: all

all:
	@mkdir -p build
	$(CC) main.c game_util.c lib/libraylib.a \
		-I ./include -L ./lib -lm -o build/main.o \
		-Wall -Wextra -std=c11 -DDEBUG

clean:
	rm build/*
