# Makefile

# Compiler
CC = g++

# Compiler Flags
CFLAGS = -std=c++17 -Wall -pthread

# Source Files
SOURCES = RingBuffer.cpp Sequencer.cpp Event.cpp Producer.cpp EventProcessor.cpp Barrier.cpp YieldWaitStrategy.cpp Disruptor.cpp main.cpp

# Object Files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
EXECUTABLE = disruptor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)

