# Target: Name of the executable
TARGET := goldenratiopyramid

# Source files
SRC := goldenratiopyramid.c

# Compiler flags
CFLAGS := -O0 -g3 

# Libraries
LDLIBS := -lSDL2 -lSDL2_image -lm

# Default target: Builds the executable
all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

run : goldenratiopyramid
	./goldenratiopyramid

# Clean target
clean:
	rm -f $(TARGET)


.PHONY: all run clean configure install

build: goldenratiopyramid
	$(MAKE) goldenratiopyramid
