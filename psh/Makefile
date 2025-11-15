# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=gnu++17

# Target executable
TARGET := psh

# Source files
SRCS := shell.cpp debug/DebugHelper.cpp internalcommands/CommandHolder.cpp internalcommands/ChangeDirectoryInternalCommand.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link objects into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
