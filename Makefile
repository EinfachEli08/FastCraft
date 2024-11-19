TARGET := FastCraft

LIB := lib
SRC := $(wildcard src/**/*.cpp src/**/*.c src/*.cpp src/*.c)
SRC += src/glad.c  # Include glad.c explicitly in the source list

# Convert the source files into object files, placing them directly in the build/ directory
OBJ := $(patsubst src/%, build/%, $(SRC:.cpp=.o) $(SRC:.c=.o))

CXX := g++

LIBS := -lglfw3dll

ARGS := -g --std=c++17 -Isrc -Iinclude -L$(LIB) $(LIBS)



# Ensure all necessary directories exist
$(shell mkdir -p build)
$(shell mkdir -p $(sort $(dir $(OBJ))))  # Create directories for all object files

# Main target: link all object files into the final executable
all: $(OBJ)
	@echo Final linking
	@$(CXX) $(ARGS) $(OBJ) -o bin/$(TARGET)

# Clean the build directory
clean:
	rm -rf bin/$(TARGET) build

# Rule for compiling .cpp files into .o files in the build/ directory
build/%.o: src/%.cpp
	@echo Compiling $<
	@mkdir -p $(dir $@)  # Ensure the directory for this .o file exists
	@$(CXX) $(ARGS) -c $< -o $@

# Rule for compiling .c files into .o files in the build/ directory
build/%.o: src/%.c
	@echo Compiling $<
	@mkdir -p $(dir $@)  # Ensure the directory for this .o file exists
	@$(CXX) $(ARGS) -c $< -o $@

# Explicit rule for compiling glad.c
build/glad.o: src/glad.c
	@echo Compiling glad.c
	@mkdir -p $(dir $@)  # Ensure the directory for this .o file exists
	@$(CXX) $(ARGS) -c $< -o $@

%.o:
	@echo $@
