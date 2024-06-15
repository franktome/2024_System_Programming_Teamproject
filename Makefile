# Makefile for compiling client.c and qr_detect.cpp

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I. $(shell pkg-config --cflags opencv4) -pthread

# Libraries
LIBS = $(shell pkg-config --libs opencv4) -lzbar -lwiringPi

# Source files
SRCS = client.c qr_detect.cpp

# Output executable
TARGET = client

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRCS)
	$(CXX) -o $(TARGET) $(SRCS) $(CXXFLAGS) $(LIBS)

# Clean target
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean