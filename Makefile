TARGET := FastCraft

LIB := lib
SRC := $(wildcard src/**/*.cpp src/**/*.c src/*.cpp src/*.c)

# Convert the source files into object files, placing them directly in the build/ directory
OBJ := $(patsubst src/%,build/%, $(SRC:.cpp=.o))
OBJ := $(patsubst src/%,build/%, $(OBJ:.c=.o))

CXX := x86_64-w64-mingw32-g++

LIBS := -lglfw3dll -lopengl32

ARGS := -g -std=c++17 -I/mingw64/include -Isrc -Iinclude -L$(LIB)

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