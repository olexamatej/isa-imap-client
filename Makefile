# Compiler
CC := g++
# Compiler flags
CFLAGS := -std=c++2a -g 

LDFLAGS= -pthread -lssl -lcrypto

# Source files
SRCS := main.cpp client.cpp runner.cpp commands.cpp connection.cpp arg_parser.cpp file_manager.cpp

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
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

run:
	make
	./imapcl imap.centrum.sk -T -p 993 -a oo -o aa
	