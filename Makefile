# Compiler
CC := g++
# Compiler flags
CFLAGS := -std=c++2a -g

LDFLAGS= -pthread -lssl -lcrypto

SRC_DIR := src
# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRCS))

# Header files
HDRS := $(wildcard $(SRC_DIR)/*.h)

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
	./imapcl imap.centrum.sk -p 143 -a auth_file -o saved_emails

tar:
	tar -cvf xolexa03.tar $(wildcard Makefile *.pdf *.py README) src/