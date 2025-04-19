.PHONY: all clean

SRCS := $(wildcard src/*.cpp)
LIBS := $(wildcard lib/*.a)
OBJS := $(patsubst src/%.cpp, build/%.o, $(SRCS))

CXX ?= g++
CXXFLAGS ?= -Iinclude -Wall -Wextra --std=c++2a
LDFLAGS := -Llib
TARGET := fps

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CXX) $(LDFLAGS) $^ -o build/$@

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/* $(TARGET)
