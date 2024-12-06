TARGET := FastCraft

LIB := lib# Define a recursive wildcard function
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

# Recursively find all .c and .cpp files in src
SRC := $(call rwildcard,src/,*.c) $(call rwildcard,src/,*.cpp)

# Convert the source files into object files, placing them directly in the build/ directory
OBJ := $(patsubst src/%,build/%, $(SRC:.cpp=.o))
OBJ := $(patsubst src/%,build/%, $(OBJ:.c=.o))

CXX := x86_64-w64-mingw32-g++

LIBS := -lglfw3dll -lopengl32 -lz

ARGS := -g -std=c++17 -I/mingw64/include -Isrc -Iinclude -L$(LIB) -Os

$(shell mkdir -p build)

# Main target: link all object files into the final executable
all: $(OBJ)
	@echo Final linking
	$(CXX) $(ARGS) $(OBJ) $(LIBS) -o bin/$(TARGET)

# Clean the build directory
clean:
	rm -rf bin/$(TARGET) build

build/%.o: src/%.cpp
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(ARGS) -c $< -o $@

build/%.o: src/%.c
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(ARGS) -c $< -o $@

%.o:
	@echo $@