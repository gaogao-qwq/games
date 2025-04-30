.PHONY: all clean

SRCS = $(wildcard src/*.cpp)
LIBS = $(wildcard lib/*.a)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))
TARGET = fps

BUILD_MODE ?= RELEASE

CXX = g++

CXXFLAGS =  -Iinclude
CXXFLAGS += -Iexternal/raylib-cpp/include
CXXFLAGS += -Iexternal/raylib/src
CXXFLAGS += -Iexternal/raylib/src/external
CXXFLAGS += -Iexternal/raylib/src/glfw/include
CXXFLAGS += -Wall -Wextra --std=c++2a
ifeq ($(BUILD_MODE),DEBUG)
	CXXFLAGS += -O -ggdb
endif
ifeq ($(BUILD_MODE),RELEASE)
	CXXFLAGS += -O2
endif
LDFLAGS = -Llib

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CXX) $(LDFLAGS) $^ -o build/$@

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/* $(TARGET)
