# Compiler
CC := g++
LDFLAGS=-pthread
# Compiler flags
CFLAGS := -std=c++2a -g

# Source files
SRCS := main.cpp tcp_client.cpp runner.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Header files
HDRS := $(wildcard *.h)

# Target executable
TARGET := imapcl 
# Default target
all: $(TARGET)

# Rule to compile object files
%.o: %.cpp $(HDRS)
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $@

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

run:
	make
	./imapcl 
	