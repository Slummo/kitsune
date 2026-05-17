BUILD   := build/

CMAKE_ARGS           :=
CORES                ?= $(shell nproc 2>/dev/null || echo 4)

.PHONY: all debug runt rune clean clean-full help

all: debug

$(BUILD)/CMakeCache.txt: CMakeLists.txt
	@echo "Configuring Debug Static build..."
	@cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF $(CMAKE_ARGS)

debug: $(BUILD)/CMakeCache.txt
	@echo "Building Debug Static..."
	@cmake --build $(BUILD) -j $(CORES)

runt: debug
	@if [ -z "$(NAME)" ]; then echo "Specify NAME (e.g., make runt NAME=my_test)"; exit 1; fi
	@echo "Running Test '$(NAME)'..."
	@./$(BUILD)/bin/test_$(NAME)

rune: debug
	@if [ -z "$(NAME)" ]; then echo "Specify NAME (e.g., make rune NAME=my_example)"; exit 1; fi
	@echo "Running Example '$(NAME)'..."
	@./$(BUILD)/bin/example_$(NAME)

clean:
	@echo "Cleaning standard build outputs..."
	@rm -rf $(BUILD)/test $(BUILD)/example
	@rm -rf $(BUILD_DEBUG_SHARED)/test $(BUILD_DEBUG_SHARED)/example

clean-full:
	@echo "Cleaning build directory..."
	@rm -rf build

help:
	@echo "Usage:"
	@echo "  make                 - Build"
	@echo "  make runt NAME=x     - Build and run test_x (debug)"
	@echo "  make rune NAME=x     - Build and run example_x (debug)"
	@echo "  make clean           - Remove local object files"
	@echo "  make clean-full      - Remove all build files"